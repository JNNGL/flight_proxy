#pragma once

#include <stdint.h>

typedef enum minecraft_version {
    MINECRAFT_1_7_2,
    MINECRAFT_1_7_6,
    MINECRAFT_1_8,
    MINECRAFT_1_9,
    MINECRAFT_1_9_1,
    MINECRAFT_1_9_2,
    MINECRAFT_1_9_4,
    MINECRAFT_1_10,
    MINECRAFT_1_11,
    MINECRAFT_1_11_1,
    MINECRAFT_1_12,
    MINECRAFT_1_12_1,
    MINECRAFT_1_12_2,
    MINECRAFT_1_13,
    MINECRAFT_1_13_1,
    MINECRAFT_1_13_2,
    MINECRAFT_1_14,
    MINECRAFT_1_14_1,
    MINECRAFT_1_14_2,
    MINECRAFT_1_14_3,
    MINECRAFT_1_14_4,
    MINECRAFT_1_15,
    MINECRAFT_1_15_1,
    MINECRAFT_1_15_2,
    MINECRAFT_1_16,
    MINECRAFT_1_16_1,
    MINECRAFT_1_16_2,
    MINECRAFT_1_16_3,
    MINECRAFT_1_16_4,
    MINECRAFT_1_17,
    MINECRAFT_1_17_1,
    MINECRAFT_1_18,
    MINECRAFT_1_18_2,
    MINECRAFT_1_19,
    MINECRAFT_1_19_1,
    MINECRAFT_1_19_3,
    MINECRAFT_1_19_4,
    MINECRAFT_1_20,
} minecraft_version_t;

#define MINECRAFT_MINIMUM MINECRAFT_1_7_2
#define MINECRAFT_LATEST MINECRAFT_1_20

typedef struct minecraft_version_data_s {
    uint32_t protocol_id;
    const char* version_name;
    minecraft_version_t version;
} minecraft_version_data_t;

extern const minecraft_version_data_t MINECRAFT_VERSIONS[];

const minecraft_version_data_t* get_version_by_protocol(uint32_t protocol);
const minecraft_version_data_t* get_version_by_name(const char* name);