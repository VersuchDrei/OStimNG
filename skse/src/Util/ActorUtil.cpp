#include "ActorUtil.h"

#include "ObjectRefUtil.h"

namespace ActorUtil {
    void setVehicle(RE::Actor* actor, RE::TESObjectREFR* vehicle) {
        SetVehicle(nullptr, 0, actor, vehicle);
    }

    void equipItem(RE::Actor* actor, RE::TESForm* item, bool preventRemoval, bool silent) {
        EquipItem(nullptr, 0, actor, item, preventRemoval, silent);
    }

    void equipItem(RE::Actor* actor, RE::TESForm* item) {
        EquipItem(nullptr, 0, actor, item, false, true);
    }

    void unequipItem(RE::Actor* actor, RE::TESForm* item, bool preventEquip, bool silent) {
        UnequipItem(nullptr, 0, actor, item, preventEquip, silent);
    }

    void unequipItem(RE::Actor* actor, RE::TESForm* item) {
        UnequipItem(nullptr, 0, actor, item, false, true);
    }

    void equipItemEx(RE::Actor* actor, RE::TESForm* item, int slotId, bool preventUnequip, bool equipSound) {
        //TODO: reimplement SKSE function here
        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto handle = skyrimVM->handlePolicy.GetHandleForObject(static_cast<RE::VMTypeID>(actor->FORMTYPE), actor);
            auto args = RE::MakeFunctionArguments(std::move(item), std::move(slotId), std::move(preventUnequip), std::move(equipSound));
            vm->DispatchMethodCall2(handle, "Actor", "EquipItemEx", args, callback);
        }
    }

    void equipItemEx(RE::Actor* actor, RE::TESForm* item, int slotId) {
        equipItemEx(actor, item, slotId, false, true);
    }

    void equipItemEx(RE::Actor* actor, RE::TESForm* item) {
        equipItemEx(actor, item, 0, false, true);
    }

    float getHeelOffset(RE::Actor* actor) {
        auto& weightModel = actor->GetBiped(0);
        if (weightModel) {
            std::set<RE::NiAVObject*> touched;

            for (int i = 0; i < 42; ++i) {
                auto& data = weightModel->objects[i];
                if (data.partClone) {
                    RE::TESForm* bipedArmor = data.item;

                    // only check slot 37, as too many objects cause weird crashed here and heel offsets should only be on the shoes anyways
                    if (bipedArmor->formType != RE::TESObjectARMO::FORMTYPE || !data.addon->HasPartOf(RE::BIPED_MODEL::BipedObjectSlot::kFeet)){
                        continue;
                    }

                    RE::NiAVObject* object = data.partClone.get();
                    
                    if (!touched.count(object)) {
                        auto clone = data.partClone.get();

                        RE::BSTriShape* shape = clone->AsTriShape();
                        if (shape) {
                            float offset = getHeelOffset(shape);
                            if (offset != 0) {
                                return offset;
                            }
                        }

                        RE::NiNode* node = clone->AsNode();
                        if (node) {
                            for (auto& child : node->GetChildren()) {
                                float offset = getHeelOffset(child.get());
                                if (offset != 0) {
                                    return offset;
                                }
                            }
                        }

                        touched.emplace(object);
                    }
                }
            }
        }

        return 0;
    }

    float getHeelOffset(RE::NiAVObject* object) {
        if (object->HasExtraData("HH_OFFSET")) {
            auto hh_offset = object->GetExtraData<RE::NiFloatExtraData>("HH_OFFSET");
            if (hh_offset) {
                return hh_offset->value;
            }
        } else if (object->HasExtraData("SDTA")) {
            auto sdta = object->GetExtraData<RE::NiStringExtraData>("SDTA");
            if (sdta) {
                json json = json::parse(sdta->value, nullptr, false);

                if (!json.is_discarded()) {
                    for (auto& element : json) {
                        if (element.contains("name") && element["name"] == "NPC" && element.contains("pos")) {
                            return element["pos"][2];
                        }
                    }
                }
            }
        }
        return 0;
    }
}