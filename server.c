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

static bool read_all(int connfd, void *buf, size_t n)
{
    while (n > 0) {
        ssize_t rv = read(connfd, buf, n);
        if (rv <= 0) {
            return false;
        }
        n -= rv;
        buf += rv;
    }
    return true;
}

static bool write_all(int connfd, void *buf, size_t n)
{
    while (n > 0) {
        ssize_t rv = write(connfd, buf, n);
        if (rv <= 0) {
            return false;
        }
        n -= rv;
        buf += rv;
    }
    return true;
}


static bool handle_merge(int connfd, Program *program, Msg *msg)
{
    bool rv;

    rv = read_all(connfd, msg->v, msg->size * sizeof(Instruction));
    if (!rv) {
        printf("[ERROR]: Failed read()\n");
        return false;
    }

    rv = program_merge_msg(program, msg);
    if (!rv) {
        printf("[ERROR]: Failed program_merge_msg()\n");
        return false;
    }

    ResultMsg res = {0};
    rv = write_all(connfd, &res, 4);
    if (!rv) {
        printf("[ERROR]: Failed write()\n");
        return false;
    }

    return true;
}

static bool handle_exec(int connfd, Program *program)
{
    Vm vm = {0};
    vm_init(&vm, program);
    loop(&vm);

    ResultMsg res = {vm.data[0]};
    bool rv = write_all(connfd, &res, 4);
    if (!rv) {
        printf("[ERROR]: Failed write()\n");
        return false;
    }

    return true;
}

static bool handle_reset(int connfd, Program *program)
{
    ResultMsg res = {
        program_clear(program) ? 0 : 1
    };

    bool rv = write_all(connfd, &res, 4);
    if (!rv) {
        printf("[ERROR]: Failed write()\n");
        return false;
    }

    return true;
}

static bool handle_connection(int connfd, Program *program)
{
    bool rv;
    int32_t type;
    uint32_t size;

    rv = read_all(connfd, &type, 4);
    if (!rv) {
        return false;
    }

    rv = read_all(connfd, &size, 4);
    if (!rv) {
        die("Failed read()\n");
        return false;
    }

    switch (type) {
        case MERGE:
            printf("[INFO]: Merging message of size %u.\n", size);
            Msg msg = (Msg) {
                .type = type,
                .size = size,
            };
            handle_merge(connfd, program, &msg);
            break;
        case EXEC:
            printf("[INFO]: Executing program of size %zu.\n", program->size);
            handle_exec(connfd, program);
            break;
        case RESET:
            printf("[INFO]: Clearing %zu instructions.\n", program->size);
            handle_reset(connfd, program);
            break;
        default:
            printf("[ERROR]: Method of type %d unknown.\n", type);
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
    addr.sin_port = ntohs(8080);        // port 8080
    addr.sin_addr.s_addr = ntohl(0);    // addr 0.0.0.0
    int rv = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rv < 0) {
        die("Failed bind()\n");
    }

    rv = listen(fd, SOMAXCONN);

    Program program;
    program_init(&program);

    printf("[INFO]: Listening on port 8080...\n");
    while (1) {
        struct sockaddr_in client_addr = {0};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0) {
            continue;
        }

        while (handle_connection(connfd, &program));

        close(connfd);
    }

    close(fd);

    return 0;
}
