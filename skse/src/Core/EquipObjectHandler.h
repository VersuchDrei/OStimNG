#pragma once

#include "GameAPI/GameActor.h"
#include "Trait/EquipObject.h"

namespace OStim {
    struct EquipObjectHandler {
    public:
        GameAPI::GameActor actor;
        Trait::EquipObject* object;
        RE::TESObjectARMO* equipped = nullptr;
        int variantDuration = 0;
        std::string variant = "";
        std::set<RE::TESObjectARMO*> toRemove;

        void equip(GameAPI::GameActor actor);
        void unequip(GameAPI::GameActor actor);
        bool setVariant(GameAPI::GameActor actor, std::string variant, int duration);
        void unsetVariant(GameAPI::GameActor actor);
        void removeItems(GameAPI::GameActor actor);

        inline void setVariant(GameAPI::GameActor actor, std::string variant) {
            setVariant(actor, variant, 0);
        }

    private:
        void equipInner(GameAPI::GameActor actor);
        void unequipInner(GameAPI::GameActor actor);
    };
}