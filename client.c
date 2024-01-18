#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "program.h"
#include "msg.h"

static void die(char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}

static bool read_all(int fd, void *buf, size_t n)
{
    while (n > 0) {
        ssize_t rv = read(fd, buf, n);
        if (rv <= 0) {
            return false;
        }
        n -= rv;
        buf += rv;
    }
    return true;
}

static bool write_all(int fd, void *buf, size_t n)
{
    while (n > 0) {
        ssize_t rv = write(fd, buf, n);
        if (rv <= 0) {
            return false;
        }
        n -= rv;
        buf += rv;
    }
    return true;
}

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

    program_add(&program, i1);
    program_add(&program, i2);
    program_add(&program, i3);
    program_add(&program, i4);
    program_add(&program, i5);
    program_add(&program, i6);
    program_add(&program, i7);

    // MERGE message
    size_t count = 0;
    RequestMsg msg;
    ResultMsg res;

    while (split_program(&program, &msg)) {
        write_all(fd, &msg, sizeof(RequestMsgH) + msg_size(&msg) * sizeof(Instruction));
        count++;
    }

    // Results
    for (size_t i = 0; i < count; i++) {
        read_all(fd, &res, sizeof(ResultMsg));
    }

    // EXEC message
    msg = (RequestMsg) {{EXEC}};
    write_all(fd, &msg, sizeof(RequestMsgH));

    // Result
    read_all(fd, &res, sizeof(ResultMsg));
    printf("msg: %d\n", res.ret);

    // RESET message
    msg = (RequestMsg) {{RESET}};
    write_all(fd, &msg, sizeof(RequestMsgH));

    close(fd);

    return 0;
}
