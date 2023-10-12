#pragma once

#include "address.h"
#include "connection.h"
#include "event_loop.h"

struct transport_s;

typedef struct listener_s {
    void(*close)(struct listener_s* listener);
    struct transport_s* transport;
    event_loop_t* event_loop;
} listener_t;

typedef struct transport_s {
    proxy_conn_t*(*connect)(struct transport_s* transport, socket_addr_t* socket);
    void(*listen)(struct transport_s* transport, event_loop_t* event_loop, socket_addr_t* socket);
} transport_t;