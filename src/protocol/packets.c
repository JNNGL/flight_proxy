#include "packets.h"

#include <stdlib.h>
#include <string.h>

packet_registry_t* global_packet_registry;

packet_registry_t* packet_registry_new() {
    packet_registry_t* registry = malloc(sizeof(packet_registry_t));
    bzero(registry, sizeof(packet_registry_t));
    return registry;
}

void packet_registry_free(packet_registry_t* registry) {
    free(registry);
}

void global_packet_registry_init() {

}

void global_packet_registry_free() {

}