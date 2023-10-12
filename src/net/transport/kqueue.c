#include "kqueue.h"

#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>

#define MAX_EVENTS 64

void kqueue_conn_write(proxy_conn_t* conn, net_buf_t* buf) {

}

void kqueue_conn_close(proxy_conn_t* conn) {
    close(((kqueue_conn_t*) conn)->fd);
    minecraft_pipe_free(&conn->minecraft_pipe);
    free(conn->buffer);
    free(conn);
}

proxy_conn_t* kqueue_connect(transport_t* transport, socket_addr_t* addr) {
    return NULL;
}

void kqueue_listener_close(listener_t* listener) {
    kqueue_listener_t* kqueue_listener = (kqueue_listener_t*) listener;
    close(kqueue_listener->kqueue_fd);
    close(kqueue_listener->socket_fd);
    free(kqueue_listener);
}

void kqueue_listener_handle_conn(kqueue_listener_t* kqueue_listener, struct kevent event) {
    while (1) {
        struct sockaddr conn_addr;
        socklen_t length = sizeof(conn_addr);

        int conn_fd = accept(kqueue_listener->socket_fd, &conn_addr, &length);
        if (conn_fd == -1) {
            if (errno != EAGAIN) {
                perror("kqueue_listen");
                return;
            }

            break;
        }

        int flags = fcntl(conn_fd, F_GETFL, 0);
        fcntl(conn_fd, F_SETFL, flags | O_NONBLOCK | O_CLOEXEC);

        struct sockaddr_in* sin_addr = (struct sockaddr_in*) &conn_addr;
        struct in_addr addr_bits = sin_addr->sin_addr;

        kqueue_conn_t* kqueue_conn = malloc(sizeof(kqueue_conn_t));
        kqueue_conn->proxy_conn.write = kqueue_conn_write;
        kqueue_conn->proxy_conn.handle_data = proxy_conn_handle_data;
        kqueue_conn->proxy_conn.close = kqueue_conn_close;
        kqueue_conn->proxy_conn.listener = (listener_t*) kqueue_listener;
        kqueue_conn->proxy_conn.socket_addr.address.hostname = "";
        inet_ntop(AF_INET, &addr_bits, kqueue_conn->proxy_conn.socket_addr.address.ip, INET_ADDRSTRLEN);
        memcpy(kqueue_conn->proxy_conn.socket_addr.address.data, &addr_bits, 4);
        kqueue_conn->proxy_conn.buffer_size = net_buf_initial_capacity();
        kqueue_conn->proxy_conn.buffer = malloc(kqueue_conn->proxy_conn.buffer_size);
        kqueue_conn->proxy_conn.socket_addr.port = htons(sin_addr->sin_port);
        minecraft_pipe_init(&kqueue_conn->proxy_conn.minecraft_pipe);
        kqueue_conn->fd = conn_fd;

        event_loop_t* event_loop = kqueue_listener->listener.event_loop;
        event_loop->handle_conn(event_loop, (proxy_conn_t*) kqueue_conn);

        char buf[100];
        socket_to_string(&kqueue_conn->proxy_conn.socket_addr, buf, 100);
        printf("connection from: %s\n", buf);
    }
}

void kqueue_listen(transport_t* transport, event_loop_t* event_loop, socket_addr_t* addr) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("kqueue_listen");
        return;
    }

    int fionbio = 1;
    if (ioctl(socket_fd, FIONBIO, &fionbio)) {
        perror("kqueue_listen");
        return;
    }

    struct sockaddr_in sockaddr = {};
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(addr->port);
    sockaddr.sin_addr.s_addr = inet_addr(addr->address.ip);

    if (bind(socket_fd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) == -1) {
        perror("kqueue_listen");
        return;
    }

    if (listen(socket_fd, SOMAXCONN) == -1) {
        perror("kqueue_listen");
        return;
    }

    int kqueue_fd = kqueue();
    if (kqueue_fd == -1) {
        perror("kqueue_listen");
        return;
    }

    struct kevent ctl_event = {};
    EV_SET(&ctl_event, socket_fd, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, NULL);

    if (kevent(kqueue_fd, &ctl_event, 1, NULL, 0, NULL) == -1) {
        perror("kqueue_listen");
        return;
    }

    kqueue_listener_t* kqueue_listener = malloc(sizeof(kqueue_listener_t));
    kqueue_listener->listener.close = kqueue_listener_close;
    kqueue_listener->listener.transport = transport;
    kqueue_listener->listener.event_loop = event_loop;
    kqueue_listener->socket_fd = socket_fd;
    kqueue_listener->kqueue_fd = kqueue_fd;

    struct kevent events[MAX_EVENTS];

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        int count = kevent(kqueue_fd, NULL, 0, events, MAX_EVENTS, NULL);

        for (int i = 0; i < count; i++) {
            struct kevent event = events[i];

            if (!(event.flags & EVFILT_READ)) {
                perror("kqueue_listen");
                continue;
            }

            kqueue_listener_handle_conn(kqueue_listener, event);
        }
    }
#pragma clang diagnostic pop
}

transport_t* kqueue_transport() {
    static transport_t transport = {};
    transport.connect = kqueue_connect;
    transport.listen = kqueue_listen;
    return &transport;
}