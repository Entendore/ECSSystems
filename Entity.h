#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include <cstdint>
#include <compare>
#include <limits>

namespace ecs {

struct Entity {
    uint32_t id;       // Unique entity index, 0 reserved as invalid/null
    uint32_t version;  // Version to avoid stale references

    constexpr Entity() noexcept : id(0), version(0) {}
    constexpr Entity(uint32_t id_, uint32_t version_) noexcept : id(id_), version(version_) {}

    [[nodiscard]] constexpr bool operator==(Entity other) const noexcept = default;
    [[nodiscard]] constexpr std::strong_ordering operator<=>(Entity other) const noexcept = default;

    [[nodiscard]] constexpr bool IsValid() const noexcept {
        return id != 0;
    }

    [[nodiscard]] static constexpr Entity Null() noexcept {
        return Entity{0, 0};
    }

    // Reset entity to null state
    void Reset() noexcept {
        id = 0;
        version = 0;
    }

    // Helper to check if entity matches another ignoring version (same id only)
    [[nodiscard]] constexpr bool HasSameId(Entity other) const noexcept {
        return id == other.id;
    }

    // Get maximum valid id (reserved 0 as invalid)
    [[nodiscard]] static constexpr uint32_t MaxId() noexcept {
        return std::numeric_limits<uint32_t>::max();
    }
};

} // namespace ecs

#endif // ECS_ENTITY_H
