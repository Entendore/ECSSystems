#ifndef ECS_H
#define ECS_H

#include "Entity.h"
#include "ComponentType.h"
#include "Chunk.h"
#include "Archetype.h"

#define MAX_ENTITIES 10000
#define MAX_ARCHETYPES 256

struct ECS {
    struct Archetype archetypes[MAX_ARCHETYPES];
    int archetypeCount;
    
    struct Entity entityPool[MAX_ENTITIES];
    int entityCount;
};

#endif // ECS_H
