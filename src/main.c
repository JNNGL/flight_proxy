#include "net/transport/kqueue.h"
#include "net/event_loop/kqueue.h"

#include "config/config.h"

#include "protocol/version.h"

#include <pthread.h>
#include <stdio.h>

void* event_loop_thread_fn(void* data) {
    event_loop_t* event_loop = (event_loop_t*) data;
    event_loop->start(event_loop);
    return NULL;
}

int main() {
    test_cfg_t* cfg = test_cfg_new();
    cfg->hello = 123;
    printf("%s\n", cfg_serialize_yaml(cfg)->buffer);
    test_cfg_free(cfg);

    transport_t* transport = kqueue_transport();
    event_loop_t* event_loop = kqueue_event_loop();

    socket_addr_t bind_addr;
    ip4_resolve(&bind_addr.address, "0.0.0.0");
    bind_addr.port = 25577;

    pthread_t thread;
    int error = pthread_create(&thread, NULL, event_loop_thread_fn, event_loop);
    if (error) {
        fprintf(stderr, "main: pthread_create failed\n");
        return -1;
    }

    pthread_detach(thread);

    char addr_str[38];
    socket_to_string(&bind_addr, addr_str, sizeof(addr_str));
    printf("Listening on %s\n", addr_str);

    transport->listen(transport, event_loop, &bind_addr);

    return 0;
}
