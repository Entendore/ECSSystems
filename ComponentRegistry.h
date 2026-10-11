#ifndef ECS_COMPONENT_REGISTRY_H
#define ECS_COMPONENT_REGISTRY_H

#include "ComponentType.h"
#include "ComponentInfo.h"
#include <vector>
#include <string_view>
#include <string>
#include <unordered_map>
#include <mutex>
#include <optional>

namespace ecs {

class ComponentRegistry {
private:
    std::vector<ComponentInfo> m_components;
    std::unordered_map<std::string, ComponentTypeId> m_nameToId;
    std::mutex m_mutex;

    ComponentRegistry() = default;

public:
    ComponentRegistry(const ComponentRegistry&) = delete;
    ComponentRegistry& operator=(const ComponentRegistry&) = delete;

    static ComponentRegistry& Instance();

    // Registers a new component with type info and returns its ID
    ComponentTypeId RegisterComponent(size_t size, size_t alignment, std::string_view name, std::string_view description = "");

    // Retrieves ComponentInfo by ID
    const ComponentInfo* GetComponentInfo(ComponentTypeId typeId) const noexcept;

    // Retrieves ComponentInfo by name
    const ComponentInfo* GetComponentInfo(std::string_view name) const;

    // Gets ComponentTypeId by name if registered
    std::optional<ComponentTypeId> GetComponentTypeIdByName(std::string_view name) const;

    // Returns all registered components
    const std::vector<ComponentInfo>& GetAllComponentInfos() const noexcept;

    // Clears all registered components (for testing or hot-reload)
    void Clear();
};

} // namespace ecs

#endif // ECS_COMPONENT_REGISTRY_H
