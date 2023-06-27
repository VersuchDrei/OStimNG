#include "EquipObjectHandler.h"

#include "Util/ActorUtil.h"
#include "Util/Constants.h"
#include "Util/ObjectRefUtil.h"

namespace OStim {
    void EquipObjectHandler::equip(GameAPI::GameActor actor) {
        if (equipped) {
            return;
        }

        equipInner(actor);
    }

    void EquipObjectHandler::unequip(GameAPI::GameActor actor) {
        if (!equipped) {
            return;
        }

        unequipInner(actor);
    }

    bool EquipObjectHandler::setVariant(GameAPI::GameActor actor, std::string variant, int duration) {
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

    void EquipObjectHandler::unsetVariant(GameAPI::GameActor actor) {
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

    void EquipObjectHandler::removeItems(GameAPI::GameActor actor) {
        for (RE::TESObjectARMO* item : toRemove) {
            // TODO properly use GameActor
            ObjectRefUtil::removeItem(actor.form, item, 1, true, nullptr);
        }
    }

    void EquipObjectHandler::equipInner(GameAPI::GameActor actor) {
        equipped = !variant.empty() ? object->variants[variant] : object->item;

        // TODO properly use GameActor
        ActorUtil::equipItem(actor.form, equipped);
        // if we remove an item mid scene the NPC will redress
        // so we have to store all the equipped items to remove them at scene end
        toRemove.insert(equipped);

        // if we don't add a slight delay here we end up with a race condition where the tongue sometimes doesn't appear
        // TODO properly use GameActor
        GameAPI::GameActor temp = actor.form;
        std::thread ninodeThread = std::thread([temp] {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            temp.update3D();
        });
        ninodeThread.detach();
    }

    void EquipObjectHandler::unequipInner(GameAPI::GameActor actor) {
        // TODO properly use GameActor
        ActorUtil::unequipItem(actor.form, equipped);

        equipped = nullptr;

        actor.update3D();
    }
}