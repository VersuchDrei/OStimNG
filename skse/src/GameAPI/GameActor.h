#pragma once

#include "GameActorValue.h"
#include "GameFaction.h"
#include "GameKeyword.h"
#include "GameRace.h"
#include "GameSex.h"

namespace GameAPI {
    struct GameActor {
    public:
        inline static GameActor getPlayer() {
            return RE::PlayerCharacter::GetSingleton();
        }

        inline static GameActor fromFile(std::string mod, uint32_t formID){
            return RE::TESDataHandler::GetSingleton()->LookupForm<RE::Actor>(formID, mod);
        }

        RE::Actor* actor;

        GameActor(RE::Actor* actor);

        inline operator bool() const { return actor; }
        inline bool operator==(const GameActor other) { return actor == other.actor; }
        inline bool operator!=(const GameActor other) { return actor != other.actor; }

        uint32_t getFormID();
        uint32_t getBaseFormID();

        bool isSex(GameSex sex);
        bool isRace(GameRace race);

        float getActorValue(GameActorValue actorValue);

        bool hasKeyword(GameKeyword keyword);
        bool hasKeyword(std::string keyword);

        bool isInFaction(GameFaction faction);
        bool hasCrimeFaction(GameFaction faction);
        int getFactionRank(GameFaction faction);

        int getRelationshipRank(GameActor other);
    };
}