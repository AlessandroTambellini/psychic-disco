#ifndef EL_H
#define EL_H

#include <stdlib.h>
#include <stdbool.h>
#include <poll.h>

#include "vm.h"

typedef enum {
    CONN_REQ,
    CONN_RES,
    CONN_END,
} ConnState;

#define BUF_SIZE 512

typedef struct {
    int fd;
    ConnState state;
    uint8_t rbuf[BUF_SIZE];
    size_t rbuf_size;
    uint8_t wbuf[BUF_SIZE];
    size_t wbuf_sent;
    size_t wbuf_size;
    Vm *vm;
} Conn;

// EventLoop is used as a map from fd to Conn
typedef struct {
    size_t size;
    Conn **conn;
} EventLoop;

#define MAX_CONN 10000

bool el_init(EventLoop *el);
bool el_resize(EventLoop *el, size_t size_new);
bool el_add(EventLoop *el, int fd);
bool el_remove(EventLoop *el, int fd);
Conn *el_get(EventLoop *el, int fd);
bool pa_init(struct pollfd *pa);
bool el_get_pa(EventLoop *el, struct pollfd *pa, size_t *pa_size);
void conn_print(Conn *conn);
void el_print(EventLoop *el);

#endif
