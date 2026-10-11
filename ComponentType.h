#ifndef ECS_COMPONENT_TYPE_H
#define ECS_COMPONENT_TYPE_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <mutex>
#include <optional>

namespace ecs {

// Runtime-unique identifier for a component type
struct ComponentTypeId {
    uint32_t id;

    constexpr ComponentTypeId() noexcept : id(0) {}
    constexpr explicit ComponentTypeId(uint32_t id_) noexcept : id(id_) {}

    [[nodiscard]] constexpr bool operator==(ComponentTypeId other) const noexcept = default;
    [[nodiscard]] constexpr bool operator!=(ComponentTypeId other) const noexcept = default;
    [[nodiscard]] constexpr bool operator<(ComponentTypeId other) const noexcept {
        return id < other.id;
    }

    [[nodiscard]] constexpr bool IsValid() const noexcept {
        return id != 0;
    }
};

// Responsible for generating and tracking ComponentTypeIds based on string names
class ComponentTypeRegistry {
private:
    static std::unordered_map<std::string, ComponentTypeId> s_nameToId;
    static std::unordered_map<uint32_t, std::string> s_idToName;
    static uint32_t s_nextId;
    static std::mutex s_mutex;

public:
    // Registers a name with a new ComponentTypeId or returns existing one
    static ComponentTypeId RegisterComponent(const std::string& name);

    // Retrieve name by type id
    static std::optional<std::string> GetName(ComponentTypeId id);

    // Retrieve id by name
    static std::optional<ComponentTypeId> GetTypeId(const std::string& name);

    // Clears all registered component types (for testing or hot reload)
    static void Reset();
};

} // namespace ecs

#endif // ECS_COMPONENT_TYPE_H
