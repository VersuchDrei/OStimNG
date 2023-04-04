#include "Condition.h"

namespace Trait {
    ActorConditions ActorConditions::create(RE::Actor* actor) {
        // TODO: when actually implementing this make a nullptr meet all conditions, it's important for Migals stuff!
        ActorConditions ret;
        return ret;
    }

    std::vector<ActorConditions> ActorConditions::create(std::vector<RE::Actor*> actors) {
        std::vector<ActorConditions> ret;
        for (auto& actor : actors) {
            ret.push_back(create(actor));
        }
        return ret;
    }

    bool ActorConditions::fulfills(ActorConditions conditions) {
        return true;
    }
}