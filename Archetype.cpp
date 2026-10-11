#include "Archetype.h"
#include <cassert>
#include <cstring>

namespace ecs {

Archetype::Archetype(std::span<const ComponentInfo> componentInfos) {
    m_componentInfos.assign(componentInfos.begin(), componentInfos.end());
    for (size_t i = 0; i < m_componentInfos.size(); ++i) {
        m_componentSignature.set(m_componentInfos[i].typeId);
        m_typeToIndex[m_componentInfos[i].typeId] = static_cast<int>(i);
    }
}

const std::bitset<kMaxComponentTypes>& Archetype::GetComponentSignature() const noexcept {
    return m_componentSignature;
}

size_t Archetype::GetChunkCount() const noexcept {
    return m_chunks.size();
}

Chunk* Archetype::GetChunk(size_t index) noexcept {
    assert(index < m_chunks.size());
    return m_chunks[index].get();
}

const Chunk* Archetype::GetChunk(size_t index) const noexcept {
    assert(index < m_chunks.size());
    return m_chunks[index].get();
}

bool Archetype::AddEntity(Entity entity) {
    for (auto& chunk : m_chunks) {
        if (!chunk->IsFull()) {
            return chunk->AddEntity(entity);
        }
    }

    auto newChunk = std::make_unique<Chunk>(m_componentInfos);
    bool added = newChunk->AddEntity(entity);
    assert(added); // New chunk must succeed
    m_chunks.push_back(std::move(newChunk));
    return true;
}

bool Archetype::RemoveEntity(Entity entity) {
    for (auto& chunk : m_chunks) {
        auto entities = chunk->GetEntities();
        for (size_t i = 0; i < entities.size(); ++i) {
            if (entities[i].id == entity.id) {
                return chunk->RemoveEntity(i);
            }
        }
    }
    return false;
}

bool Archetype::MatchesQuery(const std::bitset<kMaxComponentTypes>& query) const noexcept {
    return (m_componentSignature & query) == query;
}

bool Archetype::MoveEntityToArchetype(size_t chunkIndex, size_t entityIndex, Archetype& target) {
    assert(chunkIndex < m_chunks.size());
    Chunk* sourceChunk = m_chunks[chunkIndex].get();

    Entity entity = sourceChunk->GetEntity(entityIndex);
    bool success = target.AddEntity(entity);
    if (!success) return false;

    // Transfer matching component data
    for (const auto& info : target.m_componentInfos) {
        ComponentTypeId type = info.typeId;
        int sourceIndex = GetComponentIndex(type);
        int targetIndex = target.GetComponentIndex(type);
        if (sourceIndex >= 0 && targetIndex >= 0) {
            void* srcPtr = sourceChunk->GetComponentData(sourceIndex, entityIndex);
            Chunk* lastChunk = target.m_chunks.back().get();
            void* dstPtr = lastChunk->GetComponentData(targetIndex, lastChunk->GetEntityCount() - 1);
            std::memcpy(dstPtr, srcPtr, info.size);
        }
    }

    sourceChunk->RemoveEntity(entityIndex);
    return true;
}

std::span<const ComponentInfo> Archetype::GetComponentInfos() const noexcept {
    return m_componentInfos;
}

int Archetype::GetComponentIndex(ComponentTypeId type) const {
    auto it = m_typeToIndex.find(type);
    return it != m_typeToIndex.end() ? it->second : -1;
}

} // namespace ecs
