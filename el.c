#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "el.h"

bool el_init(EventLoop *el)
{
    size_t size = 4;
    el->size = size;
    el->conn = (Conn **)calloc(size, sizeof(Conn *));
    if (!el->conn) {
        printf("Failed allocation.\n");
        return false;
    }

    return true;
}

bool el_resize(EventLoop *el, size_t size_new)
{
    size_t size_old = el->size;
    el->size = size_new;
    el->conn = (Conn **)realloc(el->conn, size_new * sizeof(Conn *));
    if (!el->conn) {
        printf("Failed reallocation.\n");
        return false;
    }

    if (size_new > size_old) {
        size_t diff = (size_new - size_old) * sizeof(Conn *);
        memset(el->conn, 0, diff);
    }

    return true;
}

bool el_add(EventLoop *el, int fd)
{
    if (fd >= el->size) {
        el_resize(el, fd+1);
    }

    Conn *conn = (Conn *)malloc(sizeof(Conn));
    if (!conn) {
        printf("Failed allocation.\n");
        return false;
    }

    conn->fd = fd;
    conn->state = CONN_REQ;
    conn->rbuf_size = 0;
    conn->wbuf_size = 0;
    conn->wbuf_sent = 0;

    Vm *vm = (Vm *)malloc(sizeof(Vm));
    vm_init(vm);
    conn->vm = vm;

    el->conn[fd] = conn;

    return true;
}

bool el_remove(EventLoop *el, int fd)
{
    close(fd);
    vm_deinit(el->conn[fd]->vm);
    free(el->conn[fd]->vm);
    el->conn[fd] = NULL;
    return true;
}

Conn *el_get(EventLoop *el, int fd)
{
    if (fd < el->size) {
        return el->conn[fd];
    } else {
        return NULL;
    }
}

bool el_get_pa(EventLoop *el, struct pollfd *pa, size_t *pa_size)
{
    size_t pos = 1;
    for (size_t fd = 0; fd < el->size; fd++) {
        Conn *conn = el->conn[fd];
        if (!conn) {
            continue;
        }

        if (pos < MAX_CONN) {
            struct pollfd pfd = {0};
            pfd.fd = fd;
            pfd.events = conn->state == CONN_REQ ? POLLIN : POLLOUT;
            pfd.events = pfd.events | POLLERR;
            pa[pos++] = pfd;
        } else {
            printf("Event loop is not able to handle this number of clients.\n");
            el_remove(el, fd);
        }
    }
    *pa_size = pos;

    return true;
}

void conn_print(Conn *conn)
{
    printf("(fd: %d state: %d) ", conn->fd, conn->state);
}

void el_print(EventLoop *el)
{
    for (size_t fd = 0; fd < el->size; fd++) {
        Conn *conn = el->conn[fd];
        if (conn) {
            conn_print(conn);
        }
    }
    printf("\n");
}
