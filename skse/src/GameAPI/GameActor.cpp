#include "GameActor.h"

namespace GameAPI {
    void GameActor::update3D() const {
        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto args = RE::MakeFunctionArguments();
            auto handle = skyrimVM->handlePolicy.GetHandleForObject(static_cast<RE::VMTypeID>(form->FORMTYPE), form);
            vm->DispatchMethodCall2(handle, "Actor", "QueueNiNodeUpdate", args, callback);
        }
    }

    bool GameActor::isSex(GameSex sex) const {
        RE::SEX actorSex = form->GetActorBase()->GetSex();
        switch (sex) {
            case MALE:
                return actorSex == RE::SEX::kMale;
            case FEMALE:
                return actorSex == RE::SEX::kFemale;
            case NONE:
                return actorSex == RE::SEX::kNone;
        }
        return false;
    }

    void GameActor::setFactionRank(GameFaction faction, int rank) const {
        for (auto& factionInfo : form->GetActorBase()->factions) {
            if (factionInfo.faction == faction.form) {
                factionInfo.rank = rank;
            }
        }
    }

    int GameActor::getFactionRank(GameFaction faction) const {
        for (RE::FACTION_RANK rank : form->GetActorBase()->factions) {
            if (rank.faction == faction.form) {
                return rank.rank;
            }
        }
        
        return -2;
    }

    int GameActor::getRelationshipRank(GameActor other) const {
        return 4 - RE::BGSRelationship::GetRelationship(form->GetActorBase(), other.form->GetActorBase())->level.underlying();
    }
}