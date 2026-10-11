#include "Chunk.h"
#include <cassert>
#include <cstring>
#include <new>

namespace ecs {

Chunk::Chunk(std::span<const ComponentInfo> componentInfos)
    : m_componentInfos(componentInfos.begin(), componentInfos.end()) {
    const size_t count = componentInfos.size();
    m_componentBuffers.resize(count);
    m_componentSizes.resize(count);
    m_componentAlignments.resize(count);
    m_componentTypes.resize(count);

    for (size_t i = 0; i < count; ++i) {
        m_componentTypes[i] = componentInfos[i].typeId;
        m_componentSizes[i] = componentInfos[i].size;
        m_componentAlignments[i] = componentInfos[i].alignment;
    }

    AllocateBuffers();
}

Chunk::~Chunk() {
    Clear();
    DeallocateBuffers();
}

void Chunk::AllocateBuffers() {
    for (size_t i = 0; i < m_componentInfos.size(); ++i) {
        size_t bufferSize = m_componentSizes[i] * kMaxEntitiesPerChunk;
        m_componentBuffers[i] = std::make_unique<std::byte[]>(bufferSize);
    }
}

void Chunk::DeallocateBuffers() {
    m_componentBuffers.clear();
}

void Chunk::ConstructComponentAt(size_t componentIndex, size_t entityIndex) {
    std::byte* ptr = m_componentBuffers[componentIndex].get() + m_componentSizes[componentIndex] * entityIndex;
    std::memset(ptr, 0, m_componentSizes[componentIndex]); // default-zero init
}

void Chunk::DestructComponentAt(size_t componentIndex, size_t entityIndex) {
    // Do nothing unless destructors are needed
    // Could support callbacks for destructors later
}

size_t Chunk::GetEntityCount() const noexcept {
    return m_entityCount;
}

size_t Chunk::GetComponentCount() const noexcept {
    return m_componentInfos.size();
}

bool Chunk::IsFull() const noexcept {
    return m_entityCount >= kMaxEntitiesPerChunk;
}

Entity Chunk::GetEntity(size_t index) const noexcept {
    assert(index < m_entityCount);
    return m_entities[index];
}

bool Chunk::AddEntity(Entity entity) {
    if (IsFull()) return false;

    m_entities[m_entityCount] = entity;
    for (size_t i = 0; i < m_componentInfos.size(); ++i) {
        ConstructComponentAt(i, m_entityCount);
    }
    ++m_entityCount;
    return true;
}

bool Chunk::RemoveEntity(size_t index) {
    if (index >= m_entityCount) return false;

    for (size_t i = 0; i < m_componentInfos.size(); ++i) {
        DestructComponentAt(i, index);
    }

    if (index != m_entityCount - 1) {
        m_entities[index] = m_entities[m_entityCount - 1];
        for (size_t i = 0; i < m_componentInfos.size(); ++i) {
            std::memcpy(
                m_componentBuffers[i].get() + index * m_componentSizes[i],
                m_componentBuffers[i].get() + (m_entityCount - 1) * m_componentSizes[i],
                m_componentSizes[i]
            );
        }
    }

    --m_entityCount;
    return true;
}

void* Chunk::GetComponentData(size_t componentTypeIndex, size_t entityIndex) noexcept {
    assert(componentTypeIndex < m_componentBuffers.size());
    assert(entityIndex < m_entityCount);
    return m_componentBuffers[componentTypeIndex].get() + m_componentSizes[componentTypeIndex] * entityIndex;
}

const void* Chunk::GetComponentData(size_t componentTypeIndex, size_t entityIndex) const noexcept {
    assert(componentTypeIndex < m_componentBuffers.size());
    assert(entityIndex < m_entityCount);
    return m_componentBuffers[componentTypeIndex].get() + m_componentSizes[componentTypeIndex] * entityIndex;
}

int Chunk::FindComponentTypeIndex(ComponentTypeId type) const noexcept {
    for (size_t i = 0; i < m_componentTypes.size(); ++i) {
        if (m_componentTypes[i] == type) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

std::span<const Entity> Chunk::GetEntities() const noexcept {
    return std::span<const Entity>(m_entities.data(), m_entityCount);
}

void Chunk::Clear() {
    for (uint32_t i = 0; i < m_entityCount; ++i) {
        for (size_t c = 0; c < m_componentInfos.size(); ++c) {
            DestructComponentAt(c, i);
        }
    }
    m_entityCount = 0;
}

bool Chunk::HasComponent(ComponentTypeId type) const noexcept {
    return FindComponentTypeIndex(type) != -1;
}

} // namespace ecs
