#pragma once

#include "GameAPI/GameSex.h"

namespace Trait {
    struct ActorConditions {
    public:
        static ActorConditions create(RE::Actor* actor);
        static std::vector<ActorConditions> create(std::vector<RE::Actor*> actors);

        GameAPI::GameSex sex = GameAPI::GameSex::NONE;

        bool fulfills(ActorConditions conditions);
    };
}