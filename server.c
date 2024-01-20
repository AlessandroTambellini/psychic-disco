#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#include "program.h"
#include "vm.h"
#include "msg.h"

static void die(char *s)
{
    fprintf(stderr, "%s", s);
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

static bool handle_merge(int connfd, Program *program, RequestHeader header)
{
    bool rv;
    Request req = { header };

    rv = read_all(connfd, req.data, req.header.size * sizeof(Instruction));
    if (!rv) {
        printf("[ERROR]: Failed read()\n");
        return false;
    }

    rv = program_merge(program, req.data, req.header.size);

    ResponseHeader res = (ResponseHeader) {
        .type = MERGE, 
        .ret = rv ? 0 : 1
    };

    rv = write_all(connfd, &res, sizeof(res));
    if (!rv) {
        printf("[ERROR]: Failed write()\n");
        return false;
    }

    return true;
}

static bool handle_exec(int connfd, Program *program, Vm *vm, RequestHeader header)
{
    loop(vm);

    ResponseHeader res = (ResponseHeader) {
        .type = EXEC,
        .ret = vm->data[0]
    };

    bool rv = write_all(connfd, &res, sizeof(res));
    if (!rv) {
        printf("[ERROR]: Failed write()\n");
        return false;
    }

    return true;
}

static bool handle_reset(int connfd, Program *program, RequestHeader header)
{
    ResponseHeader res = (ResponseHeader) {
        .type = RESET,
        .ret = program_clear(program) ? 0 : 1
    };

    bool rv = write_all(connfd, &res, sizeof(res));
    if (!rv) {
        printf("[ERROR]: Failed write()\n");
        return false;
    }

    return true;
}

static bool handle_get(int connfd, Program *program, RequestHeader header)
{
    Program copy;
    program_init(&copy);
    program_copy(&copy, program);

    Response res;
    size_t size;
    do {
        size = MIN(program_size(&copy), PAYLOAD_SIZE);
        program_split(&copy, res.data, size);
        res.header = (ResponseHeader) {
            .type = GET,
            .size = size,
            .ret = size
        };

        size_t res_size = sizeof(ResponseHeader) + size * sizeof(Instruction);
        bool rv = write_all(connfd, &res, res_size);
        if (!rv) {
            printf("[ERROR]: Failed write()\n");
            return false;
        }
    } while (size);

    program_deinit(&copy);

    return true;
}

static bool handle_delete(int connfd, Program *program, RequestHeader header)
{
    bool rv;
    rv = program_delete(program, header.start, header.size);

    ResponseHeader res = (ResponseHeader) {
        .type = DELETE,
        .ret = rv ? 0 : 1,
        .size = program_size(program)
    };

    rv = write_all(connfd, &res, sizeof(res));
    if (!rv) {
        printf("[ERROR]: Failed write()\n");
        return false;
    }

    return true;
}

static bool handle_connection(int connfd, Program *program, Vm *vm)
{
    RequestHeader header = {0};

    bool rv = read_all(connfd, &header, sizeof(header));
    if (!rv) {
        return false;
    }

    switch (header.type) {
        case MERGE:
            printf("[INFO]: Merging message of size %u.\n", header.size);
            handle_merge(connfd, program, header);
            break;
        case EXEC:
            printf("[INFO]: Executing program of size %zu.\n", program_size(program));
            handle_exec(connfd, program, vm, header);
            break;
        case RESET:
            printf("[INFO]: Clearing %zu instructions.\n", program_size(program));
            handle_reset(connfd, program, header);
            break;
        case GET:
            printf("[INFO]: Sending %zu instructions.\n", program_size(program));
            handle_get(connfd, program, header);
            break;
        case DELETE:
            printf("[INFO]: Deleting %u instructions.\n", header.size);
            handle_delete(connfd, program, header);
            break;
        default:
            printf("[ERROR]: Method of type %d unknown.\n", header.type);
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

    Vm vm;
    vm_init(&vm, &program);

    printf("[INFO]: Listening on port 8080...\n");
    while (1) {
        struct sockaddr_in client_addr = {0};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0) {
            continue;
        }

        while (handle_connection(connfd, &program, &vm));

        close(connfd);
    }

    program_deinit(&program);
    close(fd);

    return 0;
}
