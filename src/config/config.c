#include "config.h"

#define CONFIG_IMPLEMENTATION
#include "config.h"

#include <stdio.h>
#include <limits.h>

net_buf_t* cfg_serialize_yaml(void* cfg) {
    net_buf_t* buf = net_buf_alloc();
    uint32_t current_index = 0;
    char strbuf[sizeof(uint64_t) * CHAR_BIT + 1];

    while (1) {
        cfg_entry_t* entry = (cfg_entry_t*) ((uint8_t*) cfg + current_index);
        void* data_ptr = (void*) ((uint8_t*) entry + sizeof(cfg_entry_t));

        if (entry->type == CFG_END) {
            break;
        }

#define CONFIG_INT_SERIALIZER(type, data_type, format)             \
    case type:                                                     \
        net_buf_write_str(buf, entry->name);                       \
        net_buf_write_str(buf, ": ");                              \
        data_type data_type##_num = *(data_type*) data_ptr;        \
        snprintf(strbuf, sizeof(strbuf), format, data_type##_num); \
        net_buf_write_str(buf, strbuf);                            \
        break;

        switch (entry->type) {
            case CFG_TYPE_COMMENT:
                net_buf_write_str(buf, "# ");
                net_buf_write_str(buf, entry->name);
                break;

            CONFIG_INT_SERIALIZER(CFG_TYPE_INT8, int8_t, "%d")
            CONFIG_INT_SERIALIZER(CFG_TYPE_INT16, int16_t, "%d")
            CONFIG_INT_SERIALIZER(CFG_TYPE_INT32, int32_t, "%d")
            CONFIG_INT_SERIALIZER(CFG_TYPE_INT64, int64_t , "%lld")
            CONFIG_INT_SERIALIZER(CFG_TYPE_UINT8, uint8_t, "%u")
            CONFIG_INT_SERIALIZER(CFG_TYPE_UINT16, uint16_t, "%u")
            CONFIG_INT_SERIALIZER(CFG_TYPE_UINT32, uint32_t, "%u")
            CONFIG_INT_SERIALIZER(CFG_TYPE_UINT64, uint64_t , "%llu")
        }

#undef CONFIG_INT_SERIALIZER

        net_buf_write(buf, (uint8_t*) "\n", 1);
        current_index += entry->size;
    }

    return buf;
}