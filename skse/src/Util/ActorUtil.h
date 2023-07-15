#pragma once

#include "GameAPI/GameActor.h"

namespace ActorUtil {
    inline void EquipItem(RE::BSScript::IVirtualMachine* vm, RE::VMStackID stackID, RE::Actor* actor, RE::TESForm* akItem, bool abPreventRemoval, bool abSilent) {
        using func_t = decltype(EquipItem);
        REL::Relocation<func_t> func{RELOCATION_ID(53861, 54661)};
        func(vm, stackID, actor, akItem, abPreventRemoval, abSilent);
    }

    inline void UnequipItem(RE::BSScript::IVirtualMachine* vm, RE::VMStackID stackID, RE::Actor* actor, RE::TESForm* akItem, bool abPreventEquip, bool abSilent) {
        using func_t = decltype(UnequipItem);
        REL::Relocation<func_t> func{RELOCATION_ID(53950, 54774)};
        func(vm, stackID, actor, akItem, abPreventEquip, abSilent);
    }

    inline void SetAngle(RE::BSScript::IVirtualMachine* vm, RE::VMStackID stackID, RE::TESObjectREFR* object, float afAngleX, float afAngleY, float afAngleZ) {
        using func_t = decltype(SetAngle);
        REL::Relocation<func_t> func{RELOCATION_ID(55693, 56224)};
        func(vm, stackID, object, afAngleX, afAngleY, afAngleZ);
    }

    void sort(std::vector<GameAPI::GameActor>& actors, std::vector<GameAPI::GameActor>& dominantActors, int playerIndex);

    void equipItem(RE::Actor* actor, RE::TESForm* item, bool preventRemoval, bool silent);
    void equipItem(RE::Actor* actor, RE::TESForm* item);

    void unequipItem(RE::Actor* actor, RE::TESForm* item, bool preventEquip, bool silent);
    void unequipItem(RE::Actor* actor, RE::TESForm* item);

    void equipItemEx(RE::Actor* actor, RE::TESForm* item, int slotId, bool preventUnequip, bool equipSound);
    void equipItemEx(RE::Actor* actor, RE::TESForm* item, int slotId);
    void equipItemEx(RE::Actor* actor, RE::TESForm* item);

    float getHeelOffset(RE::Actor* actor);
    float getHeelOffset(RE::NiAVObject* object);
}  // namespace ActorUtil