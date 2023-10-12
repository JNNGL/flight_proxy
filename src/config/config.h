#if !defined(CONFIG_HEADER_GUARD__) || defined(CONFIG_IMPLEMENTATION)
#undef CONFIG_HEADER_GUARD__
#define CONFIG_HEADER_GUARD__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../net/buf.h"

/*
 *
 * Config example:
 *
 * my_config.h:
 * --------------------------------------------------------------------------
 * #pragma once
 *
 * #include <config/config.h>
 *
 * make_cfg(my_config,
 *     cfg_comment("This is a comment")
 *     cfg_int(int, hello, 200);
 * )
 *
 * my_config.c:
 * --------------------------------------------------------------------------
 * #include "my_config.h"
 *
 * // Note: CONFIG_IMPLEMENTATION should be defined ONLY in source code file.
 * #define CONFIG_IMPLEMENTATION
 * #include "my_config.h"
 *
 * main.c:
 * --------------------------------------------------------------------------
 * #include "my_config.h"
 *
 * void main() {
 *     my_config_t* config = my_config_new();
 *     ...
 *     my_config_free(config);
 * }
 *
 */

#ifndef CONFIG_IMPLEMENTATION

typedef struct cfg_entry_s {
    uint32_t size;
    uint8_t type;
    const char* name;
} __attribute__((packed)) cfg_entry_t;

#define CFG_END 0
#define CFG_TYPE_COMMENT 1
#define CFG_TYPE_INT8 2
#define CFG_TYPE_INT16 3
#define CFG_TYPE_INT32 4
#define CFG_TYPE_INT64 5
#define CFG_TYPE_UINT8 6
#define CFG_TYPE_UINT16 7
#define CFG_TYPE_UINT32 8
#define CFG_TYPE_UINT64 9

net_buf_t* cfg_serialize_yaml(void* cfg);

#endif

#ifdef CONFIG_IMPLEMENTATION
#undef make_cfg
#define make_cfg(name, data)                           \
    name##_t* name##_new() {                           \
        name##_t* cfg = malloc(sizeof(name##_t));      \
        bzero(cfg, sizeof(name##_t));                  \
        uint32_t entry_offset = 0;                     \
        data                                           \
        return cfg;                                    \
    }                                                  \
    void name##_free(name##_t* cfg) {                  \
        free(cfg);                                     \
    }

#undef cfg_entry
#define cfg_entry(data_size, data_type, value, _type, _name)                    \
    {                                                                           \
         cfg_entry_t* entry = (cfg_entry_t*) ((uint8_t*) cfg + entry_offset);   \
         entry->size = data_size + sizeof(cfg_entry_t);                         \
         entry->type = _type;                                                   \
         entry->name = _name;                                                   \
         entry_offset += entry->size;                                           \
         if (data_size != 0) {                                                  \
             void* data_ptr = (void*) ((uint8_t*) entry + sizeof(cfg_entry_t)); \
             data_type default_value = value;                                   \
             memcpy(data_ptr, &default_value, sizeof(data_type));               \
         }                                                                      \
    }

#undef cfg_comment
#define cfg_comment(str) \
    cfg_entry(0, void*, 0, CFG_TYPE_COMMENT, str)

#undef cfg_field
#define cfg_field(cfg_type, type, name, value) \
    cfg_entry(sizeof(type), type, value, cfg_type, #name)

#undef CONFIG_IMPLEMENTATION
#else
#define make_cfg(name, data)                 \
    typedef struct name##_s {                \
        data                                 \
        cfg_entry_t __end;                   \
    } __attribute__((packed)) name##_t;      \
    name##_t* name##_new();                  \
    void name##_free(name##_t* cfg);

#define cfg_entry_(prefix, id) \
    cfg_entry_t __##prefix##id

#define cfg_entry(prefix, id) \
    cfg_entry_(prefix, id)

#define cfg_comment(str) \
    cfg_entry(entry, __COUNTER__);

#define cfg_field(cfg_type, type, name, value) \
    cfg_entry(entry, __COUNTER__); type name

#endif

#undef cfg_int8
#define cfg_int8(type, name, value) \
    cfg_field(CFG_TYPE_INT8, type, name, value)

#undef cfg_int16
#define cfg_int16(type, name, value) \
    cfg_field(CFG_TYPE_INT16, type, name, value)

#undef cfg_int32
#define cfg_int32(type, name, value) \
    cfg_field(CFG_TYPE_INT32, type, name, value)

#undef cfg_int64
#define cfg_int64(type, name, value) \
    cfg_field(CFG_TYPE_INT64, type, name, value)

#undef cfg_int
#define cfg_int(_type, name, value)                      \
    cfg_field(0;                                         \
        switch (sizeof(_type)) {                         \
            case 1: entry->type = CFG_TYPE_INT8; break;  \
            case 2: entry->type = CFG_TYPE_INT16; break; \
            case 4: entry->type = CFG_TYPE_INT32; break; \
            case 8: entry->type = CFG_TYPE_INT64; break; \
        }, _type, name, value)

#undef cfg_uint8
#define cfg_uint8(type, name, value) \
    cfg_field(CFG_TYPE_UINT8, type, name, value)

#undef cfg_uint16
#define cfg_uint16(type, name, value) \
    cfg_field(CFG_TYPE_UINT16, type, name, value)

#undef cfg_uint32
#define cfg_uint32(type, name, value) \
    cfg_field(CFG_TYPE_UINT32, type, name, value)

#undef cfg_uint64
#define cfg_uint64(type, name, value) \
    cfg_field(CFG_TYPE_UINT64, type, name, value)

#undef cfg_uint
#define cfg_uint(_type, name, value)                      \
    cfg_field(0;                                          \
        switch (sizeof(_type)) {                          \
            case 1: entry->type = CFG_TYPE_UINT8; break;  \
            case 2: entry->type = CFG_TYPE_UINT16; break; \
            case 4: entry->type = CFG_TYPE_UINT32; break; \
            case 8: entry->type = CFG_TYPE_UINT64; break; \
        }, _type, name, value)

make_cfg(test_cfg,
    cfg_comment("test comment")
    cfg_uint(uint8_t, hello, 200);
)

#endif