#include "address.h"

#include <stdio.h>

#if defined(__unix__) || defined(__APPLE__) || defined(__FreeBSD__)
#include <netdb.h>
#include <arpa/inet.h>
#else
#error "Unsupported platform."
#endif

uint32_t ip4_addr_bits(ip4_addr_t* addr) {
    return *(uint32_t*) addr->data;
}

int ip4_resolve(ip4_addr_t* addr, const char* hostname) {
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* result;
    int error = getaddrinfo(hostname, 0, &hints, &result);
    if (error) {
        return error;
    }

    addr->hostname = hostname;
    struct sockaddr_in* sockaddr = (struct sockaddr_in*) result->ai_addr;
    inet_ntop(AF_INET, &sockaddr->sin_addr, addr->ip, INET_ADDRSTRLEN);

    freeaddrinfo(result);
    return 0;
}

int ip4_to_string(ip4_addr_t* addr, char* str, size_t size) {
    return snprintf(str, size, "%s/%s", addr->hostname, addr->ip);
}

int socket_to_string(socket_addr_t* addr, char* str, size_t size) {
    return snprintf(str, size, "%s/%s:%u", addr->address.hostname, addr->address.ip, addr->port);
}