#include "Core/ThreadActor.h"

#include "Core/Thread.h"

#include "Trait/TraitTable.h"

namespace OStim {
    void ThreadActor::setExcitement(float value) {
        excitement = value;
        actor.setFactionRank(Trait::TraitTable::getExcitementFaction(), (int)excitement);
    }

    void ThreadActor::addExcitement(float value, bool respectMultiplier) {
        if (respectMultiplier) {
            excitement += value * excitementMultiplier;
        } else {
            excitement += value;
        }

        if (excitement < 0) {
            excitement = 0;
        } else if (excitement > 100) {
            excitement = 100;
        }

        actor.setFactionRank(Trait::TraitTable::getExcitementFaction(), (int)excitement);
    }

    void ThreadActor::loopExcitement() {
        if (awaitingClimax) {
            return;
        }

        if (excitement > maxExcitement) {
            if (excitementDecayCooldown > 0) {
                excitementDecayCooldown -= Constants::LOOP_TIME_MILLISECONDS;
            } else {
                excitement -= loopExcitementDecay;
                if (excitement < maxExcitement) {
                    excitement = maxExcitement;
                }
                actor.setFactionRank(Trait::TraitTable::getExcitementFaction(), (int)excitement);
            }
        } else {  // increase excitement
            excitement += loopExcitementInc;
            if (excitement > maxExcitement) {
                excitement = maxExcitement;
            }
            actor.setFactionRank(Trait::TraitTable::getExcitementFaction(), (int)excitement);
            excitementDecayCooldown = MCM::MCMTable::getExcitementDecayGracePeriod();
        }

        if (excitement >= 100) {
            orgasm(false);
        }
    }

    void ThreadActor::changeSpeedExcitement() {
        recalculateLoopExcitement();
    }

    void ThreadActor::setExcitementMultiplier(float multiplier) {
        excitementMultiplier = multiplier;
        recalculateLoopExcitement();
    }

    void ThreadActor::recalculateLoopExcitement() {
        float speedMod = 1.0 + static_cast<float>(speed) / static_cast<float>(thread->getCurrentNode()->speeds.size());
        setLoopExcitementInc(baseExcitementInc * excitementMultiplier * speedMod * Constants::LOOP_TIME_SECONDS);
    }
}