#pragma once

typedef enum conn_state {
    HANDSHAKE_STATE,
    STATUS_STATE,
    LOGIN_STATE,
    PLAY_STATE,
} conn_state_t;