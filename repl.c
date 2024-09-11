#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "program.h"
#include "server.h"
#include "utils.h"

#define CODE_SIZE 100

void merge_mode(Program *program)
{
    char code[CODE_SIZE];
    while (1) {
        memset(code, 0, CODE_SIZE);

        printf("> ");
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
}

void repl_merge(int fd)
{
    Program program;
    program_init(&program);

    merge_mode(&program);

    Request req;
    Response res;

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

    for (size_t i = 0; i < count; i++) {
        read_all(fd, &res.header, sizeof(res.header));
        read_all(fd, res.payload, res.header.size);
    }

    program_deinit(&program);
}

void repl_get(int fd)
{
    Program program;
    program_init(&program);

    size_t offset = 0;
    const size_t chunk = PAYLOAD_SIZE / sizeof(Instruction);

    Request req;
    Response res;

    do {
        req.header = (RequestHeader) {
            .type = GET,
            .size = 2 * 4
        };
        ((uint32_t *)req.payload)[0] = offset;
        ((uint32_t *)req.payload)[1] = chunk;
        write_all(fd, &req, sizeof(req.header) + req.header.size);

        read_all(fd, &res, sizeof(res.header));
        read_all(fd, res.payload, res.header.size);
        program_merge(&program, (Instruction *)res.payload, res.header.size / sizeof(Instruction));

        offset += chunk;
    } while (res.header.size > 0);

    program_print(&program);
    program_deinit(&program);
}

void repl_exec(int fd)
{
    Request req;
    req.header = (RequestHeader) {
        .type = EXEC,
        .size = 0
    };
    write_all(fd, &req, sizeof(req.header));

    Response res;
    read_all(fd, &res, sizeof(res.header));
    read_all(fd, res.payload, res.header.size);
    for (size_t i = 0; i < res.header.size / sizeof(int32_t); i++) {
        // printf("[0x%.4zx]: %d\n", i, ((int32_t *)res.payload)[i]);
        printf("%d ", ((int32_t *)res.payload)[i]);
    }
    printf("\n");
}

void repl_delete(int fd)
{
    uint32_t start, end;
    scanf("%d %d", &start, &end);

    Request req;
    req.header = (RequestHeader) {
        .type = DELETE,
        .size = 2 * 4
    };
    ((uint32_t *)req.payload)[0] = start;
    ((uint32_t *)req.payload)[1] = end;
    write_all(fd, &req, sizeof(req.header) + req.header.size);

    Response res;
    read_all(fd, &res, sizeof(res.header));
    read_all(fd, res.payload, res.header.size);
    uint32_t size = ((uint32_t *)res.payload)[0];

    printf("Deleted %d lines.\n", size);
}

void repl_help()
{
    const char *help =
        "Commands: \n"
        "   - merge: write down instructions and then merge them to the server\n"
        "       - this command will enter `merge mode`\n"
        "       - you can write assembly in this format <opcode> <dest> <arg1> <arg2>\n"
        "   - get: get the current state of the server\n"
        "   - exec: execute the current state of the server\n"
        "   - delete <start> <size>: delete <size> instructions starting from <start>\n"
        "Example usage:\n"
        "   > merge\n"
        "   # movi 0 69420 0\n"
        "   # done\n"
        "   > get\n"
        "   [0x0000]: 9 0 69420 0\n"
        "   > exec\n"
        "   69420\n";
        printf("%s", help);
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

    char buffer[CODE_SIZE];
    while (1) {
        memset(buffer, 0, CODE_SIZE);
        printf("$ ");
        scanf("%s", buffer);

        if (strcmp(buffer, "merge") == 0) {
            repl_merge(fd);
        } else if (strcmp(buffer, "get") == 0) {
            repl_get(fd);
        } else if (strcmp(buffer, "exec") == 0) {
            repl_exec(fd);
        } else if (strcmp(buffer, "delete") == 0) {
            repl_delete(fd);
        } else if (strcmp(buffer, "help") == 0) {
            repl_help();
        } else if (strcmp(buffer, "quit") == 0) {
            close(fd);
            break;
        } else {
            fprintf(stderr, "Not a valid command.\n");
        }
    }

    return 0;
}
