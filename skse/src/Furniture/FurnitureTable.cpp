#include "FurnitureTable.h"

#include "Util/StringUtil.h"

namespace Furniture {
    void FurnitureTable::setupForms() {
        auto handler = RE::TESDataHandler::GetSingleton();
        CraftingCookPot = handler->LookupForm<RE::BGSKeyword>(0x0a5cb3, "Skyrim.esm");
        DLC1isVampireCoffinHorizontal = handler->LookupForm<RE::BGSKeyword>(0x01932a, "Dawnguard.esm");
        DLC1isVampireCoffinVertical = handler->LookupForm<RE::BGSKeyword>(0x00b64d, "Dawnguard.esm");
        FurnitureCounterLeanMarker = handler->LookupForm<RE::BGSKeyword>(0x088106, "Skyrim.esm");
        FurnitureBedRoll = handler->LookupForm<RE::BGSKeyword>(0x0e4ad6, "Skyrim.esm");
        isLeanTable = handler->LookupForm<RE::BGSKeyword>(0x0c4ef2, "Skyrim.esm");
        IsTable = handler->LookupForm<RE::BGSKeyword>(0x09b9a9, "Skyrim.esm");
        isVampireCoffin = handler->LookupForm<RE::BGSKeyword>(0x0FDBE9, "Skyrim.esm");
        isWritingChair = handler->LookupForm<RE::BGSKeyword>(0x0eaa4b, "Skyrim.esm");
        RaceToScale = handler->LookupForm<RE::BGSKeyword>(0x0FD0E1, "Skyrim.esm");
        WICraftingAlchemy = handler->LookupForm<RE::BGSKeyword>(0x04f6e6, "Skyrim.esm");
        WICraftingEnchanting = handler->LookupForm<RE::BGSKeyword>(0x04f6dd, "Skyrim.esm");
        WICraftingSmithing = handler->LookupForm<RE::BGSKeyword>(0x04f680, "Skyrim.esm");

        BYOHVampireCoffinVert01 = handler->LookupForm<RE::TESFurniture>(0x000801, "HearthFires.esm");
        WallLeanMarker = handler->LookupForm<RE::TESFurniture>(0x052ff5, "Skyrim.esm");

        OStimShelfList = handler->LookupForm<RE::BGSListForm>(0x000da0, "OStim.esp");
    }

    FurnitureType FurnitureTable::getFurnitureType(std::string type) {
        StringUtil::toLower(&type);

        auto iter = furnitures.find(type);
        if (iter != furnitures.end()) {
            return iter->second;
        }

        return FurnitureType::NONE;
    }
}