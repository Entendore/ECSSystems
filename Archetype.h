#ifndef ECS_ARCHETYPE_H
#define ECS_ARCHETYPE_H

#include "Chunk.h"
#include "ComponentType.h"
#include "ComponentInfo.h"
#include "Entity.h"
#include <vector>
#include <cstdint>
#include <bitset>
#include <memory>
#include <unordered_map>

namespace ecs {

constexpr size_t kMaxComponentTypes = 256;

class Archetype {
private:
    std::bitset<kMaxComponentTypes> m_componentSignature;

    std::vector<std::unique_ptr<Chunk>> m_chunks;

    std::vector<ComponentInfo> m_componentInfos;
    std::unordered_map<ComponentTypeId, int> m_typeToIndex;

public:
    Archetype(std::span<const ComponentInfo> componentInfos);

    [[nodiscard]] const std::bitset<kMaxComponentTypes>& GetComponentSignature() const noexcept;

    [[nodiscard]] size_t GetChunkCount() const noexcept;
    [[nodiscard]] Chunk* GetChunk(size_t index) noexcept;
    [[nodiscard]] const Chunk* GetChunk(size_t index) const noexcept;

    bool AddEntity(Entity entity);
    bool RemoveEntity(Entity entity); // Needs external indexing in manager to be efficient

    [[nodiscard]] bool MatchesQuery(const std::bitset<kMaxComponentTypes>& query) const noexcept;

    // Move with data copy
    bool MoveEntityToArchetype(size_t chunkIndex, size_t entityIndex, Archetype& target);
    
    [[nodiscard]] std::span<const ComponentInfo> GetComponentInfos() const noexcept;
    [[nodiscard]] int GetComponentIndex(ComponentTypeId type) const;
};

} // namespace ecs

#endif // ECS_ARCHETYPE_H
