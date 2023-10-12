#include "registry.h"

#include <stdlib.h>
#include <string.h>

vregistry_builder_t* vregistry_builder_new() {
    vregistry_builder_t* builder = malloc(sizeof(vregistry_builder_t));
    bzero(builder->entries, sizeof(builder->entries));
    return builder;
}

void vregistry_add_data(vregistry_builder_t* builder, minecraft_version_t version, void* data) {
    builder->entries[version] = data;
}

void vregistry_builder_finalize(vregistry_builder_t* builder, void** dest) {
    void* current = NULL;

    for (minecraft_version_t version = MINECRAFT_MINIMUM; version <= MINECRAFT_LATEST; version++) {
        if (builder->entries[version]) {
            current = builder->entries[version];
        }

        dest[version] = current;
    }

    free(builder);
}