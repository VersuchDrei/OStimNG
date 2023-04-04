#pragma once

#include "Furniture.h"

namespace Furniture {
    class FurnitureTable {
    public:
        inline static RE::BGSKeyword* CraftingCookPot;
        inline static RE::BGSKeyword* DLC1isVampireCoffinHorizontal;
        inline static RE::BGSKeyword* DLC1isVampireCoffinVertical;
        inline static RE::BGSKeyword* FurnitureCounterLeanMarker;
        inline static RE::BGSKeyword* FurnitureBedRoll;
        inline static RE::BGSKeyword* isLeanTable;
        inline static RE::BGSKeyword* IsTable;
        inline static RE::BGSKeyword* isVampireCoffin;
        inline static RE::BGSKeyword* isWritingChair;
        inline static RE::BGSKeyword* RaceToScale;
        inline static RE::BGSKeyword* WICraftingAlchemy;
        inline static RE::BGSKeyword* WICraftingEnchanting;
        inline static RE::BGSKeyword* WICraftingSmithing;

        inline static RE::TESFurniture* BYOHVampireCoffinVert01;
        inline static RE::TESFurniture* WallLeanMarker;

        inline static RE::BGSListForm* OStimShelfList;

        static void setupForms();
        static FurnitureType getFurnitureType(std::string type);

        inline static std::vector<RE::FormType> clutterForms = {RE::FormType::Ammo, RE::FormType::Armor, RE::FormType::Book, RE::FormType::Ingredient, RE::FormType::KeyMaster, RE::FormType::Misc, RE::FormType::MovableStatic, RE::FormType::AlchemyItem, RE::FormType::Scroll, RE::FormType::SoulGem, RE::FormType::Weapon};

    private:
        inline static std::unordered_map<std::string, FurnitureType> furnitures{
            {"", FurnitureType::NONE},
            {"bed", FurnitureType::BED},
            {"bench", FurnitureType::BENCH},
            {"chair", FurnitureType::CHAIR},
            {"table", FurnitureType::TABLE},
            {"shelf", FurnitureType::SHELF},
            {"wall", FurnitureType::WALL},
            {"cookingpot", FurnitureType::COOKING_POT}};
    };
}