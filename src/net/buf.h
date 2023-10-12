#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct net_buf_s {
    uint8_t* buffer;
    size_t capacity;
    size_t reader_index;
    size_t writer_index;
    void* ctx;
} net_buf_t;

net_buf_t* net_buf_alloc();
net_buf_t* net_buf_alloc_ex(size_t initial_capacity);
void net_buf_wrap(net_buf_t* buf, uint8_t* data, size_t size);
void net_buf_free(net_buf_t* buf);

void net_buf_expand(net_buf_t* buf, size_t len);

void net_buf_write8(net_buf_t* buf, uint8_t data);
void net_buf_write16(net_buf_t* buf, uint16_t data);
void net_buf_write32(net_buf_t* buf, uint32_t data);
void net_buf_write64(net_buf_t* buf, uint64_t data);
void net_buf_writef(net_buf_t* buf, float data);
void net_buf_writed(net_buf_t* buf, double data);
void net_buf_write_var(net_buf_t* buf, uint32_t data);
void net_buf_write_var64(net_buf_t* buf, uint64_t data);
void net_buf_write_str(net_buf_t* buf, const char* str);
void net_buf_write(net_buf_t* buf, uint8_t* data, size_t size);

uint8_t net_buf_read8(net_buf_t* buf);
uint16_t net_buf_read16(net_buf_t* buf);
uint32_t net_buf_read32(net_buf_t* buf);
uint64_t net_buf_read64(net_buf_t* buf);
float net_buf_readf(net_buf_t* buf);
double net_buf_readd(net_buf_t* buf);
uint32_t net_buf_read_var(net_buf_t* buf);
uint64_t net_buf_read_var64(net_buf_t* buf);

size_t net_buf_initial_capacity();