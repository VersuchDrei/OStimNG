#pragma once

#include "Furniture/Furniture.h"
#include "Furniture/FurnitureTable.h"

namespace PapyrusFurniture {
    using VM = RE::BSScript::IVirtualMachine;

    std::string GetFurnitureType(RE::StaticFunctionTag*, RE::TESObjectREFR* furnitureRef) {
        return Furniture::FurnitureTable::getFurnitureType(furnitureRef, false)->id;
    }

    std::vector<RE::TESObjectREFR*> FindFurniture(RE::StaticFunctionTag*, int actorCount, RE::TESObjectREFR* centerRef, float radius, float sameFloor) {
        std::vector<std::pair<Furniture::FurnitureType*, GameAPI::GameObject>> furniture = Furniture::findFurniture(actorCount, centerRef, radius, sameFloor);
        std::vector<RE::TESObjectREFR*> ret;
        for (auto& [type, object] : furniture) {
            ret.push_back(object.form);
        }
        return ret;
    }

    std::vector<float> GetOffset(RE::StaticFunctionTag*, RE::TESObjectREFR* furnitureRef) {
        Furniture::FurnitureOffset offset = Furniture::getOffset(furnitureRef);
        return {offset.x, offset.y, offset.z, offset.rotation, offset.scale};
    }

    void ResetClutter(RE::StaticFunctionTag*, RE::TESObjectREFR* centerRef, float radius) {
        Furniture::resetClutter(centerRef, radius);
    }

    bool Bind(VM* a_vm) {
        const auto obj = "OFurniture"sv;

        BIND(GetFurnitureType);
        BIND(FindFurniture);
        BIND(GetOffset);
        BIND(ResetClutter);

        return true;
    }
}