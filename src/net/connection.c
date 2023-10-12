#include "connection.h"

void proxy_conn_handle_data(proxy_conn_t* conn, uint8_t* buffer, size_t size) {
    minecraft_pipe_handle_bytes(&conn->minecraft_pipe, buffer, size);
}