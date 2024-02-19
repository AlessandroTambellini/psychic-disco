#ifndef COMMON_H
#define COMMON_H

#define MIN(a, b) a < b ? a : b

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

void die(char *s)
{
    fprintf(stderr, "%s", s);
    exit(1);
}

bool read_all(int fd, void *buf, size_t n)
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

bool write_all(int fd, void *buf, size_t n)
{
    while (n > 0) {
        ssize_t rv = write(fd, buf, n);
        printf("written %zu\n", rv);
        if (rv <= 0) {
            return false;
        }
        n -= rv;
        buf += rv;
    }
    return true;
}

void set_nonblocking(int fd)
{
    errno = 0;
    int flags = fcntl(fd, F_GETFL, 0);
    if (errno) {
        printf("Failed fcntl()\n");
        return;
    }

    flags |= O_NONBLOCK;

    errno = 0;
    (void)fcntl(fd, F_SETFL, flags);
    if (errno) {
        printf("Failed fcntl()\n");
    }
}

#endif
