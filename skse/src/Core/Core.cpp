#include "Core.h"

#include "Trait/TraitTable.h"
#include "Util/ActorUtil.h"

namespace OStim {
    void freeActor(RE::Actor* actor, bool byGameLoad) {
        if (byGameLoad) {
            ActorUtil::setScale(actor, 1.0);
            // TODO: clear potential heel offset
        }

        Trait::TraitTable::removeFromExcitementFaction(actor);
        ActorUtil::setVehicle(actor, nullptr);
        ActorUtil::unlockActor(actor);
        
        actor->EvaluatePackage();
    }
}