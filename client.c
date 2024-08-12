#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "program.h"
#include "server.h"
#include "utils.h"

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("Failed to create connection socket\n");
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(8080);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
    // addr.sin_addr.s_addr = inet_addr("192.168.1.202");
    int rv = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rv < 0) {
        die("Failed to connect to server socket\n");
    }

    // Create program (factorial of 5)
    Program program;
    program_init(&program);

    Instruction i1 = { MOVI,    0, 5 };
    Instruction i2 = { MOV,     1, 0 };
    Instruction i3 = { ADDI,    1, 1, -1};
    Instruction i4 = { BEQI,    6, 1, 1};
    Instruction i5 = { MUL,     0, 0, 1};
    Instruction i6 = { B,       2 };
    Instruction i7 = { HALT };
    Instruction iX = { B,    0 };

    program_add(&program, i1);
    program_add(&program, iX);
    program_add(&program, iX);
    program_add(&program, i2);
    program_add(&program, i3);
    program_add(&program, i4);
    program_add(&program, i5);
    program_add(&program, i6);
    program_add(&program, i7);

    Request req;
    Response res;

    // MERGE message
    size_t count = 0;

    while (program_size(&program)) {
        size_t n = MIN(program_size(&program),
                PAYLOAD_SIZE/sizeof(Instruction)); // MIN(9, 16);

        size_t size = n * sizeof(Instruction);

        program_split(&program, (Instruction *)req.payload, n);
        req.header = (RequestHeader) {
            .type = MERGE,
            .size = size
        };
        size_t req_size = sizeof(req.header) + req.header.size;
        write_all(fd, &req, req_size);
        count++;
    }

    program_deinit(&program);

    // Responses
    for (size_t i = 0; i < count; i++) {
        read_all(fd, &res.header, sizeof(res.header));
        read_all(fd, res.payload, res.header.size);
    }

    // DELETE message
    req.header = (RequestHeader) {
        .type = DELETE,
        .size = 2 * 4
    };
    ((uint32_t *)req.payload)[0] = 1;
    ((uint32_t *)req.payload)[1] = 2;
    write_all(fd, &req, sizeof(req.header) + req.header.size);

    // Response
    read_all(fd, &res.header, sizeof(res.header));
    read_all(fd, res.payload, res.header.size);

    // GET message
    req.header = (RequestHeader) {
        .type = GET,
        .size = 2 * 4
    };
    ((uint32_t *)req.payload)[0] = 0;
    ((uint32_t *)req.payload)[1] = 4;
    write_all(fd, &req, sizeof(req.header) + req.header.size);

    // Response
    Program tmp;
    program_init(&tmp);
    read_all(fd, &res, sizeof(res.header));
    read_all(fd, res.payload, res.header.size);
    program_merge(&tmp, (Instruction *)res.payload, res.header.size / sizeof(Instruction));
    program_deinit(&tmp);

    // EXEC message
    req.header = (RequestHeader) {
        .type = EXEC,
        .size = 0
    };
    write_all(fd, &req, sizeof(req.header));

    // Response
    read_all(fd, &res, sizeof(res.header));
    read_all(fd, res.payload, res.header.size);
    printf("res: %d\n", res.payload[0]);

    // RESET message
    req.header = (RequestHeader) {
        .type = RESET,
        .size = 0
    };
    write_all(fd, &req, sizeof(req.header));

    // Response
    read_all(fd, &res, sizeof(res.header));
    read_all(fd, res.payload, res.header.size);

    close(fd);

    return 0;
}
