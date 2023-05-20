#include "GameActor.h"

namespace GameAPI {
    bool GameActor::isSex(GameSex sex) {
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

    bool GameActor::isRace(GameRace race) {
        return form->GetActorBase()->GetRace() == race.race;
    }


    float GameActor::getActorValue(GameActorValue actorValue) {
        return form->AsActorValueOwner()->GetBaseActorValue(actorValue.actorValue);
    }

    bool GameActor::isInFaction(GameFaction faction) {
        return form->IsInFaction(faction.faction);
    }

    bool GameActor::hasCrimeFaction(GameFaction faction) {
        return form->GetCrimeFaction() == faction.faction;
    }

    int GameActor::getFactionRank(GameFaction faction) {
        for (RE::FACTION_RANK rank : form->GetActorBase()->factions) {
            if (rank.faction == faction.faction) {
                return rank.rank;
            }
        }

        return -2;
    }

    int GameActor::getRelationshipRank(GameActor other) {
        return 4 - RE::BGSRelationship::GetRelationship(form->GetActorBase(), other.form->GetActorBase())->level.underlying();
    }
}