#pragma once

#include "GameAPI/GameActor.h"
#include "GameAPI/GameSex.h"

namespace Trait {
    struct ActorConditions {
    public:
        static ActorConditions create(GameAPI::GameActor actor);
        static std::vector<ActorConditions> create(std::vector<GameAPI::GameActor> actors);

        GameAPI::GameSex sex = GameAPI::GameSex::NONE;

        bool fulfills(ActorConditions conditions);
    };
}