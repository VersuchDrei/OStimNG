#pragma once

#include "Furniture/Furniture.h"
#include "Furniture/FurnitureTable.h"

namespace PapyrusFurniture {
    using VM = RE::BSScript::IVirtualMachine;

    int GetFurnitureType(RE::StaticFunctionTag*, RE::TESObjectREFR* furnitureRef) {
        return Furniture::getFurnitureType(furnitureRef, false);
    }

    int ToFurnitureTypeInt(RE::StaticFunctionTag*, std::string furnitureType) {
        return Furniture::FurnitureTable::getFurnitureType(furnitureType);
    }

    std::vector<RE::TESObjectREFR*> FindFurniture(RE::StaticFunctionTag*, int actorCount, RE::TESObjectREFR* centerRef, float radius, float sameFloor) {
        return Furniture::findFurniture(actorCount, centerRef, radius, sameFloor);
    }

    std::vector<float> GetOffset(RE::StaticFunctionTag*, RE::TESObjectREFR* furnitureRef) {
        return Furniture::getOffset(furnitureRef);
    }

    void ResetClutter(RE::StaticFunctionTag*, RE::TESObjectREFR* centerRef, float radius) {
        Furniture::resetClutter(centerRef, radius);
    }

    bool Bind(VM* a_vm) {
        const auto obj = "OFurniture"sv;

        BIND(GetFurnitureType);
        BIND(ToFurnitureTypeInt);
        BIND(FindFurniture);
        BIND(GetOffset);
        BIND(ResetClutter);

        return true;
    }
}