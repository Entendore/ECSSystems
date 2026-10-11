#ifndef ECS_COMPONENT_INFO_H
#define ECS_COMPONENT_INFO_H

#include "ComponentType.h"
#include <cstddef>
#include <string>
#include <string_view>
#include <iosfwd>

namespace ecs {

// Stores metadata about a registered component type
struct ComponentInfo {
    ComponentTypeId typeId;
    size_t size;
    size_t alignment;
    std::string name;
    std::string description;

    ComponentInfo() noexcept = default;

    ComponentInfo(ComponentTypeId id,
                  size_t sz,
                  size_t align,
                  std::string_view nm,
                  std::string_view desc = {}) noexcept
        : typeId(id),
          size(sz),
          alignment(align),
          name(nm),
          description(desc) {}

    [[nodiscard]] bool IsValid() const noexcept {
        return typeId.IsValid() && size > 0 && alignment > 0;
    }

    [[nodiscard]] bool operator==(const ComponentInfo& other) const noexcept {
        return typeId == other.typeId;
    }

    [[nodiscard]] bool operator!=(const ComponentInfo& other) const noexcept {
        return !(*this == other);
    }

    void Print(std::ostream& os) const;
};

// Output support
std::ostream& operator<<(std::ostream& os, const ComponentInfo& info);

} // namespace ecs

#endif // ECS_COMPONENT_INFO_H
