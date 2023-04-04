#pragma once

namespace Trait {
    struct EquipObject {
        std::string name;
        RE::TESObjectARMO* item;
        std::unordered_map<std::string, RE::TESObjectARMO*> variants;
    };
    
}