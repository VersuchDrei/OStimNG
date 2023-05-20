#pragma once

#include "GameActorBone.h"
#include "GameActorValue.h"
#include "GameFaction.h"
#include "GameKeyword.h"
#include "GameRace.h"
#include "GameRecord.h"
#include "GameSex.h"

namespace GameAPI {
    struct GameActor : public GameRecord<RE::Actor> {
    public:
        inline static GameActor getPlayer() {
            return RE::PlayerCharacter::GetSingleton();
        }

        inline GameActor(RE::Actor* actor) {
            form = actor;
        }

        inline uint32_t getBaseFormID() {
            return form->GetActorBase()->formID;
        }

        bool isSex(GameSex sex);
        bool isRace(GameRace race);

        float getActorValue(GameActorValue actorValue);

        inline bool hasKeyword(GameKeyword keyword) {
            return form->HasKeyword(keyword.keyword);
        }

        inline bool hasKeyword(std::string keyword) {
            return form->HasKeywordString(keyword);
        }

        bool isInFaction(GameFaction faction);
        bool hasCrimeFaction(GameFaction faction);
        int getFactionRank(GameFaction faction);

        int getRelationshipRank(GameActor other);

        inline GameActorBone getBone(std::string bone) {
            return form->GetNodeByName(bone);
        }
    };
}