#include "ComponentRegistry.h"

namespace ecs {

ComponentRegistry& ComponentRegistry::Instance() {
    static ComponentRegistry instance;
    return instance;
}

ComponentTypeId ComponentRegistry::RegisterComponent(size_t size, size_t alignment, std::string_view name, std::string_view description) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_nameToId.find(std::string(name));
    if (it != m_nameToId.end()) {
        return it->second;
    }

    ComponentTypeId newId = ComponentTypeRegistry::RegisterComponent(std::string(name));

    if (newId.id >= m_components.size()) {
        m_components.resize(newId.id + 1);
    }

    ComponentInfo info{};
    info.type = newId;
    info.size = size;
    info.alignment = alignment;
    info.name = std::string(name);
    info.description = std::string(description);

    m_components[newId.id] = std::move(info);
    m_nameToId[std::string(name)] = newId;

    return newId;
}

const ComponentInfo* ComponentRegistry::GetComponentInfo(ComponentTypeId typeId) const noexcept {
    if (typeId.id == 0 || typeId.id >= m_components.size()) {
        return nullptr;
    }
    return &m_components[typeId.id];
}

const ComponentInfo* ComponentRegistry::GetComponentInfo(std::string_view name) const {
    auto it = m_nameToId.find(std::string(name));
    if (it == m_nameToId.end()) {
        return nullptr;
    }
    return GetComponentInfo(it->second);
}

std::optional<ComponentTypeId> ComponentRegistry::GetComponentTypeIdByName(std::string_view name) const {
    auto it = m_nameToId.find(std::string(name));
    if (it != m_nameToId.end()) {
        return it->second;
    }
    return std::nullopt;
}

const std::vector<ComponentInfo>& ComponentRegistry::GetAllComponentInfos() const noexcept {
    return m_components;
}

void ComponentRegistry::Clear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_components.clear();
    m_nameToId.clear();
}

} // namespace ecs
