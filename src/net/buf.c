#include "buf.h"

#include <stdlib.h>
#include <string.h>

net_buf_t* net_buf_alloc() {
    return net_buf_alloc_ex(net_buf_initial_capacity());
}

net_buf_t* net_buf_alloc_ex(size_t initial_capacity) {
    net_buf_t* buf = malloc(sizeof(net_buf_t));
    buf->capacity = initial_capacity;
    buf->buffer = malloc(buf->capacity);
    buf->reader_index = 0;
    buf->writer_index = 0;
    buf->ctx = 0;
    return buf;
}

void net_buf_wrap(net_buf_t* buf, uint8_t* data, size_t size) {
    buf->buffer = data;
    buf->reader_index = 0;
    buf->writer_index = size;
    buf->capacity = size;
    buf->ctx = NULL;
}

void net_buf_free(net_buf_t* buf) {
    free(buf->buffer);
    free(buf);
}

void net_buf_expand(net_buf_t* buf, size_t len) {
    uint8_t* new_buffer = NULL;
    new_buffer = realloc(buf->buffer, buf->capacity + len);

    if (new_buffer) {
        buf->buffer = new_buffer;
        buf->capacity += len;
    }
}

void net_buf_write8(net_buf_t* buf, uint8_t data) {
    if (buf->writer_index >= buf->capacity) {
        net_buf_expand(buf, net_buf_initial_capacity());
    }

    buf->buffer[buf->writer_index++] = data;
}

void net_buf_write16(net_buf_t* buf, uint16_t data) {
    net_buf_write8(buf, (uint32_t) (data >> 8));
    net_buf_write8(buf, (uint32_t) data);
}

void net_buf_write32(net_buf_t* buf, uint32_t data) {
    net_buf_write16(buf, (uint32_t) (data >> 16));
    net_buf_write16(buf, (uint32_t) data);
}

void net_buf_write64(net_buf_t* buf, uint64_t data) {
    net_buf_write32(buf, (uint32_t) (data >> 32));
    net_buf_write32(buf, (uint32_t) data);
}

void net_buf_writef(net_buf_t* buf, float data) {
    net_buf_write32(buf, *(uint32_t*) &data);
}

void net_buf_writed(net_buf_t* buf, double data) {
    net_buf_write64(buf, *(uint64_t*) &data);
}

void net_buf_write_var(net_buf_t* buf, uint32_t data) {
    while (1) {
        if ((data & ~0x7F) == 0) {
            net_buf_write8(buf, data);
            return;
        }

        net_buf_write8(buf, (data & 0x7F) | 0x80);
        data >>= 7;
    }
}

void net_buf_write_var64(net_buf_t* buf, uint64_t data) {
    while (1) {
        if ((data & ~0x7FULL) == 0) {
            net_buf_write8(buf, data);
            return;
        }

        net_buf_write8(buf, (data & 0x7F) | 0x80);
        data >>= 7;
    }
}

void net_buf_write_str(net_buf_t* buf, const char* str) {
    net_buf_write(buf, (uint8_t*) str, strlen(str));
}

void net_buf_write(net_buf_t* buf, uint8_t* data, size_t size) {
    size_t writable_bytes = buf->capacity - buf->writer_index;
    if (size > writable_bytes) {
        size_t required_bytes = size - writable_bytes;
        size_t expand_size = required_bytes;
        net_buf_expand(buf, expand_size);
    }

    memcpy(buf->buffer + buf->writer_index, data, size);
    buf->writer_index += size;
}

uint8_t net_buf_read8(net_buf_t* buf) {
    if (buf->reader_index > buf->writer_index) {
        return 0;
    }

    return buf->buffer[buf->reader_index++];
}

uint16_t net_buf_read16(net_buf_t* buf) {
    return (uint16_t) net_buf_read8(buf) << 8 | net_buf_read8(buf);
}

uint32_t net_buf_read32(net_buf_t* buf) {
    return (uint32_t) net_buf_read16(buf) << 16 | net_buf_read16(buf);
}

uint64_t net_buf_read64(net_buf_t* buf) {
    return (uint64_t) net_buf_read32(buf) << 32 | net_buf_read32(buf);
}

float net_buf_readf(net_buf_t* buf) {
    uint32_t data = net_buf_read32(buf);
    return *(float*) &data;
}

double net_buf_readd(net_buf_t* buf) {
    uint64_t data = net_buf_read64(buf);
    return *(double*) &data;
}

uint32_t net_buf_read_var(net_buf_t* buf) {
    uint32_t value = 0;
    uint32_t position = 0;

    while (1) {
        uint8_t byte = net_buf_read8(buf);
        value |= (byte & 0x7F) << position;

        if ((byte & 0x80) == 0) {
            break;
        }

        if (position += 7 >= 32) {
            return 0;
        }
    }

    return value;
}

uint64_t net_buf_read_var64(net_buf_t* buf) {
    uint64_t value = 0;
    uint32_t position = 0;

    while (1) {
        uint8_t byte = net_buf_read8(buf);
        value |= (uint64_t) (byte & 0x7F) << position;

        if ((byte & 0x80) == 0) {
            break;
        }

        if (position += 7 >= 64)  {
            return 0;
        }
    }

    return value;
}

size_t net_buf_initial_capacity() {
    return 1024;
}