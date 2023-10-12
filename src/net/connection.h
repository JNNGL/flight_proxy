#pragma once

#include "buf.h"
#include "address.h"
#include "minecraft_pipe.h"

struct listener_s;

typedef struct proxy_conn_s {
    void(*write)(struct proxy_conn_s* conn, net_buf_t* buf);
    void(*handle_data)(struct proxy_conn_s* conn, uint8_t* buffer, size_t size);
    void(*close)(struct proxy_conn_s* conn);
    struct listener_s* listener;
    socket_addr_t socket_addr;
    uint8_t* buffer;
    size_t buffer_size;
    minecraft_pipe_t minecraft_pipe;
} proxy_conn_t;

void proxy_conn_handle_data(proxy_conn_t* conn, uint8_t* buffer, size_t size);