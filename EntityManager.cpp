#include "EntityManager.h"

namespace ecs {

EntityManager::EntityManager()
    : m_entities(), m_freeIds() { }

Entity EntityManager::CreateEntity() {
    uint32_t id;
    if (!m_freeIds.empty()) {
        id = m_freeIds.front();
        m_freeIds.pop();
        m_entities[id].alive = true;
        // Version stays the same (incremented on destruction)
    } else {
        id = static_cast<uint32_t>(m_entities.size());
        m_entities.push_back(EntityRecord{0, true});
    }
    return Entity{id, m_entities[id].version};
}

bool EntityManager::DestroyEntity(Entity entity) {
    uint32_t id = entity.id;
    if (!IsValidId(id)) return false;

    EntityRecord& record = m_entities[id];
    if (!record.alive) return false;
    if (record.version != entity.version) return false;

    // Mark dead and increment version to invalidate old handles
    record.alive = false;
    ++record.version;
    m_freeIds.push(id);

    return true;
}

bool EntityManager::IsAlive(Entity entity) const noexcept {
    uint32_t id = entity.id;
    if (!IsValidId(id)) return false;

    const EntityRecord& record = m_entities[id];
    return record.alive && (record.version == entity.version);
}

bool EntityManager::IsValidId(uint32_t id) const noexcept {
    return id < m_entities.size();
}

uint32_t EntityManager::GetVersion(uint32_t id) const noexcept {
    if (!IsValidId(id)) return 0;
    return m_entities[id].version;
}

size_t EntityManager::GetEntityCount() const noexcept {
    return m_entities.size();
}

size_t EntityManager::GetAliveCount() const noexcept {
    size_t count = 0;
    for (const auto& record : m_entities) {
        if (record.alive) ++count;
    }
    return count;
}

void EntityManager::Clear() {
    m_entities.clear();
    // Clear free ID queue
    std::queue<uint32_t> empty;
    std::swap(m_freeIds, empty);
}

} // namespace ecs
