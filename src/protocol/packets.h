#pragma once

#include "version.h"

#define C_HANDSHAKE_PACKETS 0x00
#define S_HANDSHAKE_PACKETS 0x01

#define C_STATUS_PACKETS 0x02
#define S_STATUS_PACKETS 0x02

#define C_LOGIN_PACKETS 0x05
#define S_LOGIN_PACKETS 0x03

#define C_PLAY_PACKETS 0x6F
#define S_PLAY_PACKETS 0x33

typedef struct packet_s {

} packet_t;

typedef packet_t* versioned_packet_ptr_t[MINECRAFT_LATEST + 1];

typedef struct packet_registry_s {
    versioned_packet_ptr_t c_handshake[C_HANDSHAKE_PACKETS];
    versioned_packet_ptr_t s_handshake[S_HANDSHAKE_PACKETS];
    versioned_packet_ptr_t c_status[C_STATUS_PACKETS];
    versioned_packet_ptr_t s_status[S_STATUS_PACKETS];
    versioned_packet_ptr_t c_login[C_LOGIN_PACKETS];
    versioned_packet_ptr_t s_login[S_LOGIN_PACKETS];
    versioned_packet_ptr_t c_play[C_PLAY_PACKETS];
    versioned_packet_ptr_t s_play[S_PLAY_PACKETS];
} packet_registry_t;

typedef struct packet_registry_state_s {
    versioned_packet_ptr_t* c_current;
    versioned_packet_ptr_t* s_current;
} packet_registry_state_t;

extern packet_registry_t* global_packet_registry;

packet_registry_t* packet_registry_new();
void packet_registry_free(packet_registry_t* registry);

void global_packet_registry_init();
void global_packet_registry_free();