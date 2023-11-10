#include "Core.h"

#include "ThreadManager.h"

#include "MCM/MCMTable.h"
#include "Trait/TraitTable.h"
#include "Util/ActorUtil.h"
#include "Util/APITable.h"

namespace OStim {
    void freeActor(GameAPI::GameActor actor, bool byGameLoad) {
        if (byGameLoad) {
            if (!MCM::MCMTable::isScalingDisabled()) {
                actor.setScale(1.0);
            }
            // TODO: clear potential heel offset
        }

        actor.removeFromFaction(Util::APITable::getActorCountFaction());
        actor.removeFromFaction(Util::APITable::getExcitementFaction());
        actor.removeFromFaction(Util::APITable::getSchlongifiedFaction());
        actor.removeFromFaction(Util::APITable::getTimesClimaxedFaction());
        actor.removeFromFaction(Util::APITable::getTimeUntilClimaxFaction());
        actor.unlock();
        
        actor.updateAI();

        actor.playAnimation("SOSFlaccid");
    }

    bool isEligible(GameAPI::GameActor actor) {
        return !actor.isDisabled() && !actor.isDeleted() && !actor.isChild() && !actor.isDead() && Trait::TraitTable::getActorType(actor) != "" && !ThreadManager::GetSingleton()->findActor(actor);
    }
}