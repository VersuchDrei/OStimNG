#include "Condition.h"

#include "MCM/MCMTable.h"

namespace Trait {
    ActorConditions ActorConditions::create(RE::Actor* actor) {
        // TODO: when actually implementing this make a nullptr meet all conditions, it's important for Migals stuff!
        ActorConditions conditions;

        if (actor) {
            conditions.sex = GameAPI::GameSexAPI::fromGame(actor->GetActorBase()->GetSex());
        }

        return conditions;
    }

    std::vector<ActorConditions> ActorConditions::create(std::vector<RE::Actor*> actors) {
        std::vector<ActorConditions> ret;
        for (auto& actor : actors) {
            ret.push_back(create(actor));
        }
        return ret;
    }

    bool ActorConditions::fulfills(ActorConditions other) {
        if (MCM::MCMTable::intendedSexOnly() && sex != GameAPI::GameSex::NONE && other.sex != GameAPI::GameSex::NONE && sex != other.sex) {
            return false;
        }

        return true;
    }
}