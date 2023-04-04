#pragma once

namespace Trait {
    struct ActorConditions {
    public:
        static ActorConditions create(RE::Actor* actor);
        static std::vector<ActorConditions> create(std::vector<RE::Actor*> actors);

        bool fulfills(ActorConditions conditions);
    };
}