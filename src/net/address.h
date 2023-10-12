#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct ip4_addr_s {
    const char* hostname;
    char ip[16];
    uint8_t data[4];
} ip4_addr_t;

typedef struct socket_addr_s {
    ip4_addr_t address;
    uint16_t port;
} socket_addr_t;

uint32_t ip4_addr_bits(ip4_addr_t* addr);
int ip4_resolve(ip4_addr_t* addr, const char* hostname);
int ip4_to_string(ip4_addr_t* addr, char* str, size_t size);
int socket_to_string(socket_addr_t* addr, char* str, size_t size);