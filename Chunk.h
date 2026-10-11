#ifndef ECS_CHUNK_H
#define ECS_CHUNK_H

#include "ComponentType.h"
#include "ComponentInfo.h"
#include "Entity.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <span>
#include <array>

namespace ecs {

class Chunk {
public:
    static constexpr size_t kMaxEntitiesPerChunk = 64;

private:
    uint32_t m_entityCount = 0;
    std::vector<ComponentInfo> m_componentInfos;

    std::vector<std::unique_ptr<std::byte[]>> m_componentBuffers;
    std::vector<size_t> m_componentSizes;
    std::vector<size_t> m_componentAlignments;
    std::vector<ComponentTypeId> m_componentTypes;

    std::array<Entity, kMaxEntitiesPerChunk> m_entities;

    void AllocateBuffers();
    void DeallocateBuffers();
    void ConstructComponentAt(size_t componentIndex, size_t entityIndex);
    void DestructComponentAt(size_t componentIndex, size_t entityIndex);

public:
    Chunk(std::span<const ComponentInfo> componentInfos);
    ~Chunk();

    [[nodiscard]] size_t GetEntityCount() const noexcept;
    [[nodiscard]] size_t GetComponentCount() const noexcept;
    [[nodiscard]] bool IsFull() const noexcept;

    [[nodiscard]] Entity GetEntity(size_t index) const noexcept;

    bool AddEntity(Entity entity);
    bool RemoveEntity(size_t index);

    [[nodiscard]] void* GetComponentData(size_t componentTypeIndex, size_t entityIndex) noexcept;
    [[nodiscard]] const void* GetComponentData(size_t componentTypeIndex, size_t entityIndex) const noexcept;

    [[nodiscard]] int FindComponentTypeIndex(ComponentTypeId type) const noexcept;

    [[nodiscard]] std::span<const Entity> GetEntities() const noexcept;

    void Clear();

    [[nodiscard]] bool HasComponent(ComponentTypeId type) const noexcept;
};

} // namespace ecs

#endif // ECS_CHUNK_H
