#include "Core.h"

#include "ThreadManager.h"

#include "MCM/MCMTable.h"
#include "Trait/TraitTable.h"
#include "Util/ActorUtil.h"

namespace OStim {
    void freeActor(GameAPI::GameActor actor, bool byGameLoad) {
        if (byGameLoad) {
            if (!MCM::MCMTable::isScalingDisabled()) {
                actor.setScale(1.0);
            }
            // TODO: clear potential heel offset
        }

        actor.removeFromFaction(Trait::TraitTable::getExcitementFaction());
        // TODO properly use GameActor
        ActorUtil::setVehicle(actor.form, nullptr);
        ActorUtil::unlockActor(actor.form);
        
        actor.updateAI();
    }

    bool isEligible(GameAPI::GameActor actor) {
        return actor.isHuman() && !ThreadManager::GetSingleton()->findActor(actor);
    }
}