#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "program.h"

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

    // MERGE message (factorial of 5)
    Msg msg1 = {0};
    msg1.type = MERGE;
    msg1.size = 7;

    Instruction i1 = { MOVI,    0, 5 };
    Instruction i2 = { MOV,     1, 0 };
    Instruction i3 = { ADDI,    1, 1, -1};
    Instruction i4 = { BEQI,    6, 1, 1};
    Instruction i5 = { MUL,     0, 0, 1};
    Instruction i6 = { B,       2 };
    Instruction i7 = { HALT };

    msg1.v[0] = i1;
    msg1.v[1] = i2;
    msg1.v[2] = i3;
    msg1.v[3] = i4;
    msg1.v[4] = i5;
    msg1.v[5] = i6;
    msg1.v[6] = i7;

    write_all(fd, &msg1, 4 + 4 + msg1.size * sizeof(Instruction));

    // EXEC message
    Msg msg2 = {0};
    msg2.type = EXEC;
    write_all(fd, &msg2, 4 + 4);

    // RESET message
    Msg msg3 = {0};
    msg3.type = RESET;
    write_all(fd, &msg3, 4 + 4);

    // Read ResultMsg
    ResultMsg res = {0};

    read_all(fd, &res, 4);
    printf("Result1: %d\n", res.ret);

    read_all(fd, &res, 4);
    printf("Result2: %d\n", res.ret);

    read_all(fd, &res, 4);
    printf("Result3: %d\n", res.ret);

    close(fd);

    return 0;
}
