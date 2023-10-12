#pragma once

#include "../event_loop.h"

#include <sys/event.h>

typedef struct kqueue_event_loop_s {
    event_loop_t event_loop;
    int kqueue_fd;
    struct kevent event;
} kqueue_event_loop_t;

event_loop_t* kqueue_event_loop();