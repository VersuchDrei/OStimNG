#pragma once

#include "Trait/EquipObject.h"

namespace OStim {
    struct EquipObjectHandler {
    public:
        RE::Actor* actor;
        Trait::EquipObject* object;
        RE::TESObjectARMO* equipped = nullptr;
        int variantDuration = 0;
        std::string variant = "";
        std::set<RE::TESObjectARMO*> toRemove;

        void equip(RE::Actor* actor);
        void unequip(RE::Actor* actor);
        bool setVariant(RE::Actor* actor, std::string variant, int duration);
        void unsetVariant(RE::Actor* actor);
        void removeItems(RE::Actor* actor);

        inline void setVariant(RE::Actor* actor, std::string variant) {
            setVariant(actor, variant, 0);
        }

    private:
        void equipInner(RE::Actor* actor);
        void unequipInner(RE::Actor* actor);
    };
}