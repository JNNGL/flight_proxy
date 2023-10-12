#include "version.h"

#include <string.h>

const minecraft_version_data_t MINECRAFT_VERSIONS[] = {
        {.protocol_id = 4, .version_name = "1.7.2", .version = MINECRAFT_1_7_2},
        {.protocol_id = 5, .version_name = "1.7.6", .version = MINECRAFT_1_7_6},
        {.protocol_id = 47, .version_name = "1.8", .version = MINECRAFT_1_8},
        {.protocol_id = 107, .version_name = "1.9", .version = MINECRAFT_1_9},
        {.protocol_id = 108, .version_name = "1.9.1", .version = MINECRAFT_1_9_1},
        {.protocol_id = 109, .version_name = "1.9.2", .version = MINECRAFT_1_9_2},
        {.protocol_id = 110, .version_name = "1.9.4", .version = MINECRAFT_1_9_4},
        {.protocol_id = 210, .version_name = "1.10", .version = MINECRAFT_1_10},
        {.protocol_id = 315, .version_name = "1.11", .version = MINECRAFT_1_11},
        {.protocol_id = 316, .version_name = "1.11.1", .version = MINECRAFT_1_11_1},
        {.protocol_id = 335, .version_name = "1.12", .version = MINECRAFT_1_12},
        {.protocol_id = 338, .version_name = "1.12.1", .version = MINECRAFT_1_12_1},
        {.protocol_id = 340, .version_name = "1.12.2", .version = MINECRAFT_1_12_2},
        {.protocol_id = 393, .version_name = "1.13", .version = MINECRAFT_1_13},
        {.protocol_id = 401, .version_name = "1.13.1", .version = MINECRAFT_1_13_1},
        {.protocol_id = 404, .version_name = "1.13.2", .version = MINECRAFT_1_13_2},
        {.protocol_id = 477, .version_name = "1.14", .version = MINECRAFT_1_14},
        {.protocol_id = 480, .version_name = "1.14.1", .version = MINECRAFT_1_14_1},
        {.protocol_id = 485, .version_name = "1.14.2", .version = MINECRAFT_1_14_2},
        {.protocol_id = 490, .version_name = "1.14.3", .version = MINECRAFT_1_14_3},
        {.protocol_id = 498, .version_name = "1.14.4", .version = MINECRAFT_1_14_4},
        {.protocol_id = 573, .version_name = "1.15", .version = MINECRAFT_1_15},
        {.protocol_id = 575, .version_name = "1.15.1", .version = MINECRAFT_1_15_1},
        {.protocol_id = 578, .version_name = "1.15.2", .version = MINECRAFT_1_15_2},
        {.protocol_id = 735, .version_name = "1.16", .version = MINECRAFT_1_16},
        {.protocol_id = 736, .version_name = "1.16.1", .version = MINECRAFT_1_16_1},
        {.protocol_id = 751, .version_name = "1.16.2", .version = MINECRAFT_1_16_2},
        {.protocol_id = 753, .version_name = "1.16.3", .version = MINECRAFT_1_16_3},
        {.protocol_id = 754, .version_name = "1.16.4", .version = MINECRAFT_1_16_4},
        {.protocol_id = 755, .version_name = "1.17", .version = MINECRAFT_1_17},
        {.protocol_id = 756, .version_name = "1.17.1", .version = MINECRAFT_1_17_1},
        {.protocol_id = 757, .version_name = "1.18", .version = MINECRAFT_1_18},
        {.protocol_id = 758, .version_name = "1.18.2", .version = MINECRAFT_1_18_2},
        {.protocol_id = 759, .version_name = "1.19", .version = MINECRAFT_1_19},
        {.protocol_id = 760, .version_name = "1.19.1", .version = MINECRAFT_1_19_1},
        {.protocol_id = 761, .version_name = "1.19.3", .version = MINECRAFT_1_19_3},
        {.protocol_id = 762, .version_name = "1.19.4", .version = MINECRAFT_1_19_4},
        {.protocol_id = 763, .version_name = "1.20", .version = MINECRAFT_1_20},
};

const minecraft_version_data_t* get_version_by_protocol(uint32_t protocol) {
    const minecraft_version_data_t* current_version = MINECRAFT_VERSIONS;
    while (1) {
        if (current_version->protocol_id == protocol) {
            return current_version;
        }

        if (current_version->version != MINECRAFT_LATEST) {
            current_version++;
        } else {
            return NULL;
        }
    }
}

const minecraft_version_data_t* get_version_by_name(const char* name) {
    char version[16];
    if (strncmp(name, "MINECRAFT_", 10) == 0) {
        strcpy(version, name + 10);

        char* current = version;
        while ((current = strchr(current, '_')) != NULL) {
            *current++ = '.';
        }
    } else {
        strcpy(version, name);
    }

    const minecraft_version_data_t* current_version = MINECRAFT_VERSIONS;
    while (1) {
        if (strcmp(current_version->version_name, version) == 0) {
            return current_version;
        }

        if (current_version->version != MINECRAFT_LATEST) {
            current_version++;
        } else {
            return NULL;
        }
    }
}