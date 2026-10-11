#include "ComponentType.h"

namespace ecs {

std::unordered_map<std::string, ComponentTypeId> ComponentTypeRegistry::s_nameToId;
std::unordered_map<uint32_t, std::string> ComponentTypeRegistry::s_idToName;
uint32_t ComponentTypeRegistry::s_nextId = 0;
std::mutex ComponentTypeRegistry::s_mutex;

ComponentTypeId ComponentTypeRegistry::RegisterComponent(const std::string& name) {
    std::lock_guard<std::mutex> lock(s_mutex);

    auto it = s_nameToId.find(name);
    if (it != s_nameToId.end()) {
        return it->second;
    }

    ComponentTypeId id(++s_nextId);
    s_nameToId[name] = id;
    s_idToName[id.id] = name;
    return id;
}

std::optional<std::string> ComponentTypeRegistry::GetName(ComponentTypeId id) {
    std::lock_guard<std::mutex> lock(s_mutex);
    auto it = s_idToName.find(id.id);
    if (it != s_idToName.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<ComponentTypeId> ComponentTypeRegistry::GetTypeId(const std::string& name) {
    std::lock_guard<std::mutex> lock(s_mutex);
    auto it = s_nameToId.find(name);
    if (it != s_nameToId.end()) {
        return it->second;
    }
    return std::nullopt;
}

void ComponentTypeRegistry::Reset() {
    std::lock_guard<std::mutex> lock(s_mutex);
    s_nameToId.clear();
    s_idToName.clear();
    s_nextId = 0;
}

} // namespace ecs
