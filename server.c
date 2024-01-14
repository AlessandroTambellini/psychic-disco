#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#include "program.h"
#include "vm.h"

static void die(char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}

static bool read_all(int connfd, int *buf, size_t n)
{
    while (n > 0) {
        ssize_t rv = read(connfd, buf, n);
        if (rv <= 0) {
            return false;
        }
        n -= rv;
        buf += rv;
    }
    return 0;
}

static bool handle_connection(int connfd, Program *program)
{
    bool rv;
    Msg msg = {0};
    Vm vm = {0};

    rv = read_all(connfd, (int *)&msg, 8);
    if (!rv) {
        die("Failed read()\n");
        return false;
    }

    switch (msg.type) {
        case MERGE:
            rv = read_all(connfd, (int *)msg.v, msg.size);
            if (!rv) {
                die("Failed read()\n");
                return false;
            }
            program_merge_msg(program, &msg);
            break;
        case EXEC:
            vm_init(&vm, program);
            loop(&vm);
            break;
        case RESET:
            // program_clear(program);
            break;
        default:
            return false;
    }

    return true;
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = 8080;               // port 8080
    addr.sin_addr.s_addr = ntohl(0);    // addr 0.0.0.0
    int rv = bind(fd, (const struct sockaddr *)&addr, (socklen_t)sizeof(addr));
    if (rv < 0) {
        die("Failed bind()\n");
    }

    rv = listen(fd, SOMAXCONN);

    Program program;
    program_init(&program);

    while (1) {
        struct sockaddr_in client_addr = {0};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0) {
            continue;
        }

        handle_connection(connfd, &program);

        close(connfd);
    }

    return 0;
}
