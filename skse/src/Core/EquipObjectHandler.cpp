#include "EquipObjectHandler.h"

#include "Util/ActorUtil.h"
#include "Util/Constants.h"
#include "Util/ObjectRefUtil.h"

namespace OStim {
    void EquipObjectHandler::equip(RE::Actor* actor) {
        if (equipped) {
            return;
        }

        equipInner(actor);
    }

    void EquipObjectHandler::unequip(RE::Actor* actor) {
        if (!equipped) {
            return;
        }

        unequipInner(actor);
    }

    bool EquipObjectHandler::setVariant(RE::Actor* actor, std::string variant, int duration) {
        if (this->variant == variant) {
            if (duration == 0) {
                variantDuration = 0;
            } else if (variantDuration != 0 && duration > variantDuration) {
                variantDuration = duration;
            }
            return true;
        }

        if (!object->variants.contains(variant)) {
            return false;
        }

        this->variant = variant;
        variantDuration = duration;

        if (equipped) {
            unequipInner(actor);
            equipInner(actor);
        }

        return true;
    }

    void EquipObjectHandler::unsetVariant(RE::Actor* actor) {
        if (variant.empty()) {
            return;
        }

        variant = "";
        variantDuration = 0;

        if (equipped) {
            unequipInner(actor);
            equipInner(actor);
        }
    }

    void EquipObjectHandler::removeItems(RE::Actor* actor) {
        for (RE::TESObjectARMO* item : toRemove) {
            ObjectRefUtil::removeItem(actor, item, 1, true, nullptr);
        }
    }

    void EquipObjectHandler::equipInner(RE::Actor* actor) {
        equipped = !variant.empty() ? object->variants[variant] : object->item;
        
        ActorUtil::equipItem(actor, equipped);
        // if we remove an item mid scene the NPC will redress
        // so we have to store all the equipped items to remove them at scene end
        toRemove.insert(equipped);

        // if we don't add a slight delay here we end up with a race condition where the tongue sometimes doesn't appear
        RE::Actor* temp = actor;
        std::thread ninodeThread = std::thread([temp] {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            ActorUtil::queueNiNodeUpdate(temp);
        });
        ninodeThread.detach();
    }

    void EquipObjectHandler::unequipInner(RE::Actor* actor) {
        ActorUtil::unequipItem(actor, equipped);

        equipped = nullptr;

        ActorUtil::queueNiNodeUpdate(actor);
    }
}