#ifndef ECS_ENTITY_MANAGER_H
#define ECS_ENTITY_MANAGER_H

#include "Entity.h"
#include <vector>
#include <cstdint>
#include <queue>

namespace ecs {

class EntityManager {
private:
    struct EntityRecord {
        uint32_t version = 0;
        bool alive = false;
    };

    std::vector<EntityRecord> m_entities;
    std::queue<uint32_t> m_freeIds;

public:
    EntityManager();

    // Create a new entity, reusing freed IDs when possible
    [[nodiscard]] Entity CreateEntity();

    // Destroy an entity, invalidates handle and recycles its ID
    bool DestroyEntity(Entity entity);

    // Check if an entity handle is valid and alive
    [[nodiscard]] bool IsAlive(Entity entity) const noexcept;

    // Check if an entity ID is valid (exists within vector bounds)
    [[nodiscard]] bool IsValidId(uint32_t id) const noexcept;

    // Get version for an entity ID (returns 0 if invalid)
    [[nodiscard]] uint32_t GetVersion(uint32_t id) const noexcept;

    // Get total number of created entities (alive + dead)
    [[nodiscard]] size_t GetEntityCount() const noexcept;

    // Get total number of alive entities
    [[nodiscard]] size_t GetAliveCount() const noexcept;

    // Clear all entities, resetting manager state
    void Clear();
};

} // namespace ecs

#endif // ECS_ENTITY_MANAGER_H
