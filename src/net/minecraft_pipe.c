#include "minecraft_pipe.h"

void minecraft_pipe_init(minecraft_pipe_t* pipe) {
    pipe->receive_buffer = net_buf_alloc();
    pipe->remaining_bytes = -1;
    pipe->version = &MINECRAFT_VERSIONS[MINECRAFT_LATEST];
}

void minecraft_pipe_free(minecraft_pipe_t* pipe) {
    net_buf_free(pipe->receive_buffer);
}

void minecraft_pipe_handle_bytes(minecraft_pipe_t* pipe, uint8_t* buffer, size_t size) {
    const uint8_t* end = buffer + size;
    while (buffer != end) {
        if (pipe->remaining_bytes == -1) {
            net_buf_t buf;
            net_buf_wrap(&buf, buffer, size);
            pipe->remaining_bytes = net_buf_read_var(&buf);
            buffer += buf.reader_index;
        }

        size_t to_write = pipe->remaining_bytes;
        if (to_write > end - buffer) {
            to_write = end - buffer;
        }

        net_buf_write(pipe->receive_buffer, buffer, to_write);
        pipe->remaining_bytes -= (ssize_t) to_write;
        buffer += to_write;

        if (pipe->remaining_bytes == 0) {
            pipe->remaining_bytes = -1;

            minecraft_pipe_handle_packet_bytes(pipe, pipe->receive_buffer);

            pipe->receive_buffer->writer_index = 0;
            pipe->receive_buffer->reader_index = 0;
        }
    }
}

void minecraft_pipe_handle_packet_bytes(minecraft_pipe_t* pipe, net_buf_t* buf) {

}