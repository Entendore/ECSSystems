#include "ECS.h"
#include "ComponentInfo.h"
#include <string.h>
#include <stdlib.h>

static struct Archetype* CreateArchetype(struct ECS* ecs, uint64_t signature) {
    if (ecs->archetypeCount >= MAX_ARCHETYPES) {
        return 0;
    }

    struct Archetype* arch = &ecs->archetypes[ecs->archetypeCount++];
    arch->signature = signature;
    arch->chunks = (struct Chunk*)malloc(sizeof(struct Chunk) * 8); // start with 8 chunks
    arch->chunkCount = 0;
    arch->chunkCapacity = 8;

    return arch;
}

static struct Archetype* FindOrCreateArchetype(struct ECS* ecs, uint64_t signature) {
    for (int i = 0; i < ecs->archetypeCount; ++i) {
        if (ecs->archetypes[i].signature == signature) {
            return &ecs->archetypes[i];
        }
    }
    return CreateArchetype(ecs, signature);
}

static struct Chunk* AllocateChunk(struct Archetype* arch) {
    if (arch->chunkCount >= arch->chunkCapacity) {
        return 0;
    }

    struct Chunk* chunk = &arch->chunks[arch->chunkCount++];
    chunk->count = 0;

    for (int i = 0; i < COMPONENT_COUNT; ++i) {
        if ((arch->signature >> i) & 1) {
            chunk->componentData[i] = malloc(gComponentInfo[i].size * MAX_ENTITIES_PER_CHUNK);
        } else {
            chunk->componentData[i] = 0;
        }
    }

    return chunk;
}

struct Entity CreateEntity(struct ECS* ecs, uint64_t componentMask) {
    if (ecs->entityCount >= MAX_ENTITIES) {
        return (Entity){0, 0};
    }

    struct Archetype* arch = FindOrCreateArchetype(ecs, componentMask);
    struct Chunk* targetChunk = 0;

    for (int i = 0; i < arch->chunkCount; ++i) {
        if (arch->chunks[i].count < MAX_ENTITIES_PER_CHUNK) {
            targetChunk = &arch->chunks[i];
            break;
        }
    }

    if (!targetChunk) {
        targetChunk = AllocateChunk(arch);
        if (!targetChunk) return (Entity){0, 0}; // failed
    }

    struct Entity e = {ecs->entityCount, 0};
    targetChunk->entities[targetChunk->count] = e;
    ++targetChunk->count;

    ecs->entityPool[ecs->entityCount++] = e;
    return e;
}
