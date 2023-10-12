#pragma once

#include "buf.h"
#include "../protocol/version.h"

#include <string.h>

typedef struct minecraft_pipe_s {
    const minecraft_version_data_t* version;
    net_buf_t* receive_buffer;
    ssize_t remaining_bytes;
} minecraft_pipe_t;

void minecraft_pipe_init(minecraft_pipe_t* pipe);
void minecraft_pipe_free(minecraft_pipe_t* pipe);

void minecraft_pipe_handle_bytes(minecraft_pipe_t* pipe, uint8_t* buffer, size_t size);
void minecraft_pipe_handle_packet_bytes(minecraft_pipe_t* pipe, net_buf_t* buf);