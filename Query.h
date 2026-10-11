#ifndef ECS_QUERY_H
#define ECS_QUERY_H

#include "Archetype.h"
#include "ComponentType.h"
#include <bitset>
#include <vector>
#include <cstddef>

namespace ecs {

class Query {
private:
    std::bitset<Archetype::kMaxComponentTypes> m_requiredComponents;
    std::bitset<Archetype::kMaxComponentTypes> m_optionalComponents;
    std::bitset<Archetype::kMaxComponentTypes> m_excludedComponents;

public:
    Query();

    // Add required component (must be present)
    void AddRequiredComponent(ecs::ComponentTypeId type);

    // Add optional component (may be present)
    void AddOptionalComponent(ecs::ComponentTypeId type);

    // Add excluded component (must NOT be present)
    void AddExcludedComponent(ecs::ComponentTypeId type);

    // Clear all query conditions
    void Reset();

    // Check if archetype matches the query conditions
    [[nodiscard]] bool MatchesArchetype(const Archetype& archetype) const noexcept;

    // Check if an entity with its archetype matches the query
    [[nodiscard]] bool MatchesEntity(const Archetype& archetype, ecs::Entity entity) const;

    // Retrieve pointers to archetypes from a list that match this query
    [[nodiscard]] std::vector<Archetype*> GetMatchingArchetypes(const std::vector<std::unique_ptr<Archetype>>& archetypes) const;

    // Iterate over matching archetypes and chunks, with a callback accepting Archetype&, Chunk&
    void ForEachMatch(const std::vector<std::unique_ptr<Archetype>>& archetypes,
                      void(*callback)(Archetype&, Chunk&)) const;

    // Helper template version for callbacks with context (e.g. lambdas)
    template<typename Func>
    void ForEachMatch(const std::vector<std::unique_ptr<Archetype>>& archetypes, Func&& callback) const;
};

template<typename Func>
void Query::ForEachMatch(const std::vector<std::unique_ptr<Archetype>>& archetypes, Func&& callback) const {
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

#endif // ECS_QUERY_H
