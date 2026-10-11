#include "Query.h"

namespace ecs {

Query::Query()
    : m_requiredComponents(), m_optionalComponents(), m_excludedComponents() {}

void Query::AddRequiredComponent(ecs::ComponentTypeId type) {
    if (type.id < Archetype::kMaxComponentTypes) {
        m_requiredComponents.set(type.id);
        // If required, remove from optional and excluded to keep consistency
        m_optionalComponents.reset(type.id);
        m_excludedComponents.reset(type.id);
    }
}

void Query::AddOptionalComponent(ecs::ComponentTypeId type) {
    if (type.id < Archetype::kMaxComponentTypes) {
        // Only add optional if not required or excluded
        if (!m_requiredComponents.test(type.id) && !m_excludedComponents.test(type.id)) {
            m_optionalComponents.set(type.id);
        }
    }
}

void Query::AddExcludedComponent(ecs::ComponentTypeId type) {
    if (type.id < Archetype::kMaxComponentTypes) {
        m_excludedComponents.set(type.id);
        // Excluded overrides required and optional
        m_requiredComponents.reset(type.id);
        m_optionalComponents.reset(type.id);
    }
}

void Query::Reset() {
    m_requiredComponents.reset();
    m_optionalComponents.reset();
    m_excludedComponents.reset();
}

bool Query::MatchesArchetype(const Archetype& archetype) const noexcept {
    const std::bitset<Archetype::kMaxComponentTypes>& signature = archetype.GetComponentSignature();

    // Required components all must be present
    if ((signature & m_requiredComponents) != m_requiredComponents) {
        return false;
    }

    // None of the excluded components must be present
    if ((signature & m_excludedComponents).any()) {
        return false;
    }

    // Optional components presence is not required, so no check needed here

    return true;
}

bool Query::MatchesEntity(const Archetype& archetype, ecs::Entity /*entity*/) const {
    // Stub: without per-entity data filtering, just archetype-based filtering
    return MatchesArchetype(archetype);
}

std::vector<Archetype*> Query::GetMatchingArchetypes(const std::vector<std::unique_ptr<Archetype>>& archetypes) const {
    std::vector<Archetype*> matches;
    for (auto& archetypePtr : archetypes) {
        if (!archetypePtr) continue;
        if (MatchesArchetype(*archetypePtr)) {
            matches.push_back(archetypePtr.get());
        }
    }
    return matches;
}

void Query::ForEachMatch(const std::vector<std::unique_ptr<Archetype>>& archetypes,
                        void(*callback)(Archetype&, Chunk&)) const {
    if (!callback) return;
    for (auto& archetypePtr : archetypes) {
        if (!archetypePtr) continue;
        if (MatchesArchetype(*archetypePtr)) {
            size_t chunkCount = archetypePtr->GetChunkCount();
            for (size_t i = 0; i < chunkCount; ++i) {
                Chunk* chunk = archetypePtr->GetChunk(i);
                if (chunk) {
                    callback(*archetypePtr, *chunk);
                }
            }
        }
    }
}

} // namespace ecs
