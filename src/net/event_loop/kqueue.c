#include "kqueue.h"

#include "../transport/kqueue.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define MAX_EVENTS 64

void kqueue_event_handle_data(kqueue_event_loop_t* kqueue_event_loop, proxy_conn_t* conn) {
    while (1) {
        if (!conn) {
            break;
        }

        kqueue_conn_t* kqueue_conn = (kqueue_conn_t*) conn;
        ssize_t count = read(kqueue_conn->fd, conn->buffer, conn->buffer_size);

        if (count == -1) {
            if (errno != EAGAIN) {
                perror("kqueue_event_handle_data");
            }

            break;
        } else if (count == 0) {
            char str[100];
            socket_to_string(&conn->socket_addr, str, 100);
            printf("connection closed: %s\n", str);
            conn->close(conn);
        } else {
            char str[100];
            socket_to_string(&conn->socket_addr, str, 100);
            printf("received %zd bytes from %s\n", count, str);
            conn->handle_data(conn, conn->buffer, count);
        }
    }
}

_Noreturn void kqueue_event_loop_start(event_loop_t* event_loop) {
    kqueue_event_loop_t* kqueue_event_loop = (kqueue_event_loop_t*) event_loop;

    struct kevent events[64];

    while (1) {
        int count = kevent(kqueue_event_loop->kqueue_fd, NULL, 0, events, MAX_EVENTS, 0);

        for (int i = 0; i < count; i++) {
            struct kevent event = events[i];
            proxy_conn_t* conn = (proxy_conn_t*) event.udata;

            if (event.flags & EV_EOF) {
                char str[100];
                socket_to_string(&conn->socket_addr, str, 100);
                printf("connection closed: %s\n", str);
                conn->close(conn);
            } else {
                kqueue_event_handle_data(kqueue_event_loop, conn);
            }
        }
    }
}

void kqueue_event_loop_handle_conn(event_loop_t* event_loop, proxy_conn_t* conn) {
    kqueue_event_loop_t* kqueue_event_loop = (kqueue_event_loop_t*) event_loop;
    kqueue_conn_t* kqueue_conn = (kqueue_conn_t*) conn;

    EV_SET(&kqueue_event_loop->event, kqueue_conn->fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kqueue_event_loop->event.udata = conn;
    if (kevent(kqueue_event_loop->kqueue_fd, &kqueue_event_loop->event, 1, NULL, 0, NULL) == -1) {
        perror("kqueue_event_loop_handle_conn");
        conn->close(conn);
    }
}

void kqueue_event_loop_close(event_loop_t* event_loop) {
    kqueue_event_loop_t* kqueue_event_loop = (kqueue_event_loop_t*) event_loop;
    close(kqueue_event_loop->kqueue_fd);
    free(kqueue_event_loop);
}

event_loop_t* kqueue_event_loop() {
    int kqueue_fd = kqueue();
    if (kqueue_fd == -1) {
        perror("kqueue_event_loop");
        return NULL;
    }

    kqueue_event_loop_t* kqueue_event_loop = malloc(sizeof(kqueue_event_loop_t));
    kqueue_event_loop->event_loop.start = kqueue_event_loop_start;
    kqueue_event_loop->event_loop.handle_conn = kqueue_event_loop_handle_conn;
    kqueue_event_loop->event_loop.close = kqueue_event_loop_close;
    kqueue_event_loop->kqueue_fd = kqueue_fd;

    int flags = fcntl(kqueue_fd, F_GETFL, 0);
    fcntl(kqueue_fd, F_SETFL, flags | O_CLOEXEC);

    EV_SET(&kqueue_event_loop->event, 0, EVFILT_READ, EV_ADD, 0, 0, NULL);

    if (kevent(kqueue_fd, &kqueue_event_loop->event, 1, NULL, 0, NULL) == -1) {
        perror("kqueue_event_loop");
        kqueue_event_loop_close((event_loop_t*) kqueue_event_loop);
        return NULL;
    }

    return (event_loop_t*) kqueue_event_loop;
}