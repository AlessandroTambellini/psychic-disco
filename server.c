#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "el.h"
#include "utils.h"
#include "server.h"

bool handle_connection(Conn *conn)
{
    switch (conn->state) {
        case CONN_REQ:
            handle_request(conn);
            break;
        case CONN_RES:
            handle_response(conn);
            break;
        case CONN_END:
        default:
            return false;
            break;
    }

    return true;
}

bool handle_request(Conn *conn)
{
    while (conn->state == CONN_REQ) {
        ssize_t bytes = 0;
        do {
            size_t n = sizeof(conn->rbuf) - conn->rbuf_size;
            bytes = read(conn->fd, conn->rbuf, n);
        } while (bytes < 0 && errno == EINTR);

        // Check if the file is ready for polling
        if (bytes < 0) {
            if (errno != EAGAIN) {
                printf("Failed to read request.\n");
                conn->state = CONN_END;
                return false;
            }
            break;
        }

        // Check if the request has finished
        if (bytes == 0) {
            if (conn->rbuf_size > 0) {
                printf("Unexpected EOF\n");
                exit(1);
                return false;
            } else {
                printf("EOF\n");
            }
            conn->state = CONN_END;
            break;
        }

        conn->rbuf_size += (size_t)bytes;

        // Use this for pipelining, allow for multiple
        // requests to be store inside rbuf simultaneously
        while (conn->rbuf_size) {
            // Check if the header is ready to be read
            if (conn->rbuf_size < sizeof(RequestHeader)) {
                break;
            }

            // Read header
            RequestHeader header;
            memcpy(&header, conn->rbuf, sizeof(header));

            // Check if the payload is ready to be read
            if (conn->rbuf_size - sizeof(header) < header.size) {
                break;
            }

            // Read payload
            Request req = { header };
            memcpy(req.payload, conn->rbuf + sizeof(header), header.size);
            // printf("payload %d\n", ((int *)req.payload)[0]);
            // printf("payload %d\n", ((int *)req.payload)[1]);

            Response res = {0};
            switch (req.header.type) {
                case MERGE:
                    handle_merge(conn, &req, &res);
                    break;
                case EXEC:
                    handle_exec(conn, &req, &res);
                    break;
                case RESET:
                    handle_reset(conn, &req, &res);
                    break;
                case GET:
                    handle_get(conn, &req, &res);
                    break;
                case DELETE:
                    handle_delete(conn, &req, &res);
                    break;
                default:
                    break;
            }

            // Copy response to wbuf
            size_t res_size = sizeof(res.header) + res.header.size;
            memcpy(conn->wbuf, &res, res_size);
            conn->wbuf_size = res_size;

            // Clear rbuf
            size_t req_size = sizeof(req.header) + req.header.size;
            size_t remain = conn->rbuf_size - req_size;
            if (remain) {
                memmove(conn->rbuf, conn->rbuf + req_size, remain);
            }
            conn->rbuf_size = remain;

            conn->state = CONN_RES;

            // If I remove this pipelining doesn't work
            handle_response(conn);
        }
    }

    return true;
}

bool handle_merge(Conn *conn, Request *req, Response *res)
{
    size_t bytes = req->header.size;
    size_t n = bytes / sizeof(Instruction);

    Program *program = conn->vm->program;
    bool rv = program_merge(program, (Instruction *)req->payload, n);
    if (!rv) {
        printf("Failed merge\n");
        res->header.status = 1;
        res->header.size = 0;
        return false;
    }

    res->header.status = 0;
    res->header.size = 0;
    return true;
}

bool handle_exec(Conn *conn, Request *req, Response *res)
{
    loop(conn->vm);
    size_t size = MIN(DATA_SIZE, PAYLOAD_SIZE);
    res->header.status = 0;
    res->header.size = size;
    memcpy(res->payload, conn->vm->data, size);
    return true;
}

bool handle_reset(Conn *conn, Request *req, Response *res)
{
    Program *program = conn->vm->program;
    bool rv = program_clear(program);
    if (!rv) {
        res->header.status = 1;
        res->header.size = 0;
        return false;
    }

    res->header.status = 69;
    res->header.size = 69;
    return true;
}

bool handle_get(Conn *conn, Request *req, Response *res)
{
    size_t start = ((uint32_t *)req->payload)[0];
    size_t size = ((uint32_t *)req->payload)[1];
    size = MIN(size, PAYLOAD_SIZE / sizeof(Instruction));

    Program *program = conn->vm->program;
    bool rv = program_get(program, (Instruction *)res->payload, start, size);
    if (!rv) {
        res->header.status = 1;
        res->header.size = 0;
        return false;
    }

    res->header.status = 0;
    res->header.size = size * sizeof(Instruction);
    return true;
}

bool handle_delete(Conn *conn, Request *req, Response *res)
{
    size_t start = ((uint32_t *)req->payload)[0];
    size_t size = ((uint32_t *)req->payload)[1];
    Program *program = conn->vm->program;
    bool rv = program_delete(program, start, size);
    if (!rv) {
        res->header.status = 1;
        res->header.size = 0;
        return false;
    }

    res->header.status = 0;
    res->header.size = 0;
    return true;
}

bool handle_response(Conn *conn)
{
    while (conn->state == CONN_RES) {
        size_t bytes = 0;
        do {
            size_t n = conn->wbuf_size - conn->wbuf_sent;
            bytes = write(conn->fd, conn->wbuf + conn->wbuf_sent, n);
        } while (bytes < 0 && errno == EINTR);

        if (bytes < 0) {
            if (errno != EAGAIN) {
                printf("Failed to write to fd.\n");
                conn->state = CONN_END;
                return false;
            }
            break;
        }

        conn->wbuf_sent += (size_t)bytes;
        assert(conn->wbuf_sent <= conn->wbuf_size);
        if (conn->wbuf_sent == conn->wbuf_size) {
            conn->state = CONN_REQ;
            conn->wbuf_size = 0;
            conn->wbuf_sent = 0;
        }
    }

    return true;
}

int main()
{
    // 1) socket()
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        printf("Failed socket()\n");
        exit(1);
    }
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // 2) bind()
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(0);
    int rv = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (rv < 0) {
        printf("Failed bind()\n");
        exit(1);
    }

    // 3) listen()
    listen(fd, SOMAXCONN);
    if (rv < 0) {
        printf("Failed listen()\n");
        exit(1);
    }

    set_nonblocking(fd);

    EventLoop el;
    el_init(&el);

    // Pollfd Array
    struct pollfd pa[MAX_CONN];
    struct pollfd pfd = {fd, POLLIN, 0};
    pa[0] = pfd;

    while (1) {
        size_t pa_size;
        if (!el_get_pa(&el, pa, &pa_size)) {
            printf("Failed to get pollfds from connections.\n");
        }

        // for (size_t i = 0; i < pa_size; i++) {
        //     printf("%d ", pa[i].fd);
        // }
        // printf("\n");
        //

        if (poll(pa, (nfds_t)pa_size, 1000) < 0) {
            printf("Failed polling operation.\n");
        }

        for (size_t i = 1; i < pa_size; i++) {
            if (pa[i].revents) {
                int fd = pa[i].fd;
                Conn *conn = el_get(&el, fd);
                handle_connection(conn);
                if (conn->state == CONN_END) {
                    el_remove(&el, fd);
                }
            }
        }

        if (pa[0].revents) {
            struct sockaddr_in client_addr = {0};
            socklen_t socklen = sizeof(client_addr);
            int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
            if (connfd < 0) {
                printf("Failed to accept new connection.\n");
                return -1;
            }

            set_nonblocking(connfd);

            el_add(&el, connfd);
        }

        // el_print(&el);
    }

    return 0;
}
