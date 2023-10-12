#pragma once

#include "connection.h"

typedef struct event_loop_s {
    void(*start)(struct event_loop_s* event_loop);
    void(*handle_conn)(struct event_loop_s* event_loop, proxy_conn_t* conn);
    void(*close)(struct event_loop_s* event_loop);
} event_loop_t;
