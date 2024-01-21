#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "program.h"
#include "msg.h"
#include "common.h"

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("Failed socket()\n");
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(8080);
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);
    int rv = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rv < 0) {
        die("Failed connect()\n");
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

    // MERGE message
    size_t count = 0;
    Request req;
    Response res;

    while (program_size(&program)) {
        size_t size = MIN(program_size(&program), PAYLOAD_SIZE);
        program_split(&program, req.data, size);
        req.header = (RequestHeader) {
            .type = MERGE,
            .size = size,
        };

        size_t req_size = sizeof(req.header) + req.header.size * sizeof(Instruction);
        write_all(fd, &req, req_size);
        count++;
    }

    program_deinit(&program);

    // Responses
    for (size_t i = 0; i < count; i++) {
        read_all(fd, &res, sizeof(res.header));
    }

    // DELETE message
    req.header = (RequestHeader) {
        .type = DELETE,
        .start = 1,
        .size = 2
    };
    write_all(fd, &req, sizeof(req.header));

    // Responses
    read_all(fd, &res, sizeof(res.header));

    // GET message
    req.header = (RequestHeader) {
        .type = GET
    };
    write_all(fd, &req, sizeof(req.header));

    // Responses
    Program tmp;
    program_init(&tmp);
    do {
        read_all(fd, &res, sizeof(res.header));
        read_all(fd, &res.data, res.header.size * sizeof(Instruction));
        program_merge(&tmp, res.data, res.header.size);
    } while (res.header.size);
    program_print(&tmp);
    program_deinit(&tmp);

    // EXEC message
    req.header = (RequestHeader) {
        .type = EXEC
    };
    write_all(fd, &req, sizeof(req.header));

    // Response
    read_all(fd, &res, sizeof(res.header));
    printf("msg: %d\n", res.header.ret);

    // RESET message
    req = (Request) {
        .header = {
            .type = RESET
        }
    };
    write_all(fd, &req, sizeof(req.header));

    // Response
    read_all(fd, &res, sizeof(res.header));

    close(fd);

    return 0;
}
