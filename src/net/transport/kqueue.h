#pragma once

#include "../transport.h"

typedef struct kqueue_conn_s {
    proxy_conn_t proxy_conn;
    int fd;
} kqueue_conn_t;

typedef struct kqueue_listener_s {
    listener_t listener;
    int kqueue_fd;
    int socket_fd;
} kqueue_listener_t;

transport_t* kqueue_transport();