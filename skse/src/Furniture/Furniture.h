#pragma once

#include "FurnitureOffset.h"
#include "FurnitureType.h"

namespace Furniture {
    std::vector<std::pair<FurnitureType*, GameAPI::GameObject>> findFurniture(int actorCount, GameAPI::GameObject centerRef, float radius, float sameFloor);
    GameAPI::GameObject findBed(RE::TESObjectREFR* centerRef, float radius, float sameFloor);
    FurnitureOffset getOffset(GameAPI::GameObject object);

    void resetClutter(RE::TESObjectREFR* centerRef, float radius);
}