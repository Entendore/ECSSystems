#include "Entity.h"
#include <string>

namespace ecs {

// Optional utility: convert entity to string (e.g. for logging)
std::string ToString(Entity entity) {
    if (!entity.IsValid()) return "Entity{Invalid}";
    return "Entity{id=" + std::to_string(entity.id) + ", version=" + std::to_string(entity.version) + "}";
}

} // namespace ecs
