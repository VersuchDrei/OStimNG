#include "GameActor.h"

namespace GameAPI {
    GameActor::GameActor(RE::Actor* actor) : actor{actor} {}
    

    uint32_t GameActor::getFormID() {
        return actor->GetFormID();
    }

    uint32_t GameActor::getBaseFormID() {
        return actor->GetActorBase()->formID;
    }


    bool GameActor::isSex(GameSex sex) {
        RE::SEX actorSex = actor->GetActorBase()->GetSex();
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
        return actor->GetActorBase()->GetRace() == race.race;
    }


    float GameActor::getActorValue(GameActorValue actorValue) {
        return actor->AsActorValueOwner()->GetBaseActorValue(actorValue.actorValue);
    }


    bool GameActor::hasKeyword(GameKeyword keyword) {
        return actor->HasKeyword(keyword.keyword);
    }

    bool GameActor::hasKeyword(std::string keyword) {
        return actor->HasKeywordString(keyword);
    }


    bool GameActor::isInFaction(GameFaction faction) {
        return actor->IsInFaction(faction.faction);
    }

    bool GameActor::hasCrimeFaction(GameFaction faction) {
        return actor->GetCrimeFaction() == faction.faction;
    }

    int GameActor::getFactionRank(GameFaction faction) {
        for (RE::FACTION_RANK rank : actor->GetActorBase()->factions) {
            if (rank.faction == faction.faction) {
                return rank.rank;
            }
        }

        return -2;
    }

    int GameActor::getRelationshipRank(GameActor other) {
        return 4 - RE::BGSRelationship::GetRelationship(actor->GetActorBase(), other.actor->GetActorBase())->level.underlying();
    }
}