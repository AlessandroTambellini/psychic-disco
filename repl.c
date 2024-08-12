#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "program.h"
#include "server.h"
#include "utils.h"

// > merge
// # add a 1
// # add b 2
// # addi 3 2
// # done
// > get
// # add a 1
// # add b 2
// # addi 3 2
// > exec
// > delete 0 3

#define CODE_SIZE 100

void merge_mode(Program *program, int fd)
{
    char code[CODE_SIZE];
    while (1) {
        memset(code, 0, CODE_SIZE);

        printf("# ");
        scanf("%s", code);
        if (strcmp(code, "done") == 0) {
            break;
        }

        Instruction inst;
        int rv = inst_decode(code, &inst.code);
        if (rv) {
            scanf("%d %d %d", &inst.dest, &inst.arg1, &inst.arg2);
            program_add(program, inst);
        } else {
            fprintf(stderr, "Not a valid instruction.\n");
        }
    }

    Request req;
    Response res;

    size_t count = 0;

    while (program_size(program)) {
        size_t n = MIN(program_size(program),
                PAYLOAD_SIZE/sizeof(Instruction)); // MIN(9, 16);

        size_t size = n * sizeof(Instruction);

        program_split(program, (Instruction *)req.payload, n);
        req.header = (RequestHeader) {
            .type = MERGE,
            .size = size
        };
        size_t req_size = sizeof(req.header) + req.header.size;
        write_all(fd, &req, req_size);
        count++;
    }

    for (size_t i = 0; i < count; i++) {
        read_all(fd, &res.header, sizeof(res.header));
        read_all(fd, res.payload, res.header.size);
    }
}

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

    Program program;
    program_init(&program);

    char buffer[CODE_SIZE];
    while (1) {
        memset(buffer, 0, CODE_SIZE);
        printf("> ");
        scanf("%s", buffer);

        if (strcmp(buffer, "merge") == 0) {
            merge_mode(&program, fd);
        } else if (strcmp(buffer, "get") == 0) {
        } else if (strcmp(buffer, "exec") == 0) {
        } else if (strcmp(buffer, "delete") == 0) {
        } else {
            fprintf(stderr, "Not a valid command.\n");
        }
    }
    return 0;
}
