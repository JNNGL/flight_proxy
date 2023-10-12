#pragma once

#include "version.h"

typedef struct vregistry_builder_s {
    void* entries[MINECRAFT_LATEST + 1];
} vregistry_builder_t;

vregistry_builder_t* vregistry_builder_new();
void vregistry_add_data(vregistry_builder_t* builder, minecraft_version_t version, void* data);
void vregistry_builder_finalize(vregistry_builder_t* builder, void** dest);