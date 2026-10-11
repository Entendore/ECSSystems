#include "ComponentInfo.h"
#include <iostream>

namespace ecs {

void ComponentInfo::Print(std::ostream& os) const {
    os << "ComponentInfo {\n"
       << "  TypeID: " << typeId.id << "\n"
       << "  Name: " << name << "\n"
       << "  Size: " << size << "\n"
       << "  Alignment: " << alignment << "\n"
       << "  Description: " << description << "\n"
       << "}";
}

std::ostream& operator<<(std::ostream& os, const ComponentInfo& info) {
    info.Print(os);
    return os;
}

} // namespace ecs
