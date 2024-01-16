#include "Condition.h"

#include "TraitTable.h"

#include "Core/ThreadActor.h"
#include "Core/ThreadManager.h"
#include "MCM/MCMTable.h"
#include "Util/APITable.h"
#include "Util/CompatibilityTable.h"

namespace Trait {
    ActorCondition ActorCondition::create(GameAPI::GameActor actor) {
        return create(actor, OStim::ThreadManager::GetSingleton()->findActor(actor));
    }

    ActorCondition ActorCondition::create(OStim::ThreadActor* actor) {
        return create(actor->getActor(), actor);
    }

    ActorCondition ActorCondition::create(GameAPI::GameActor actor, OStim::ThreadActor* threadActor) {
        ActorCondition condition;

        if (!threadActor) {
            if (Compatibility::CompatibilityTable::hasSchlong(actor)) {
                Util::APITable::getSchlongifiedFaction().add(actor);
            }
        }

        condition.requirements = Trait::TraitTable::getActorRequirements(actor);

        if (!threadActor) {
            Util::APITable::getSchlongifiedFaction().remove(actor);
        }

        // nullptr needs to meet all conditions, it's important for some add-ons
        if (!actor) {
            condition.type = "npc";
            return condition;
        }

        condition.type = TraitTable::getActorType(actor);

        GameAPI::GameSex sex = actor.getSex();
        switch (sex) {
            case GameAPI::GameSex::FEMALE:
                if (MCM::MCMTable::futaUseMaleRole() && Compatibility::CompatibilityTable::hasSchlong(actor)) {
                    condition.sex = GameAPI::GameSex::AGENDER; // despite the name this means both, not none
                } else {
                    condition.sex = GameAPI::GameSex::FEMALE;
                }
                break;
            default:
                condition.sex = sex;
            break;
        }

        return condition;
    }

    std::vector<ActorCondition> ActorCondition::create(std::vector<GameAPI::GameActor> actors) {
        std::vector<ActorCondition> ret;
        for (auto& actor : actors) {
            ret.push_back(create(actor));
        }
        return ret;
    }

    bool ActorCondition::fulfills(ActorCondition other) {
        if (type != other.type) {
            return false;
        }

        if (MCM::MCMTable::unrestrictedNavigation()) {
            return true;
        }

        if (MCM::MCMTable::intendedSexOnly() && sex != GameAPI::GameSex::AGENDER && other.sex != GameAPI::GameSex::AGENDER && sex != other.sex) {
            return false;
        }

        for (const std::string& requirement : other.requirements) {
            if (!requirements.contains(requirement)) {
                return false;
            }
        }
        
        return true;
    }
}