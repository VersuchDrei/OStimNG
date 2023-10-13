#include "GraphActor.h"

#include "Util/VectorUtil.h"

namespace Graph {
    void GraphActor::merge(ActionActor& actor) {
        condition.requirements |= actor.requirements;
        moan |= actor.moan;
        talk |= actor.talk;
        muffled |= actor.muffled;
        for (GameAPI::GameFaction faction : actor.factions) {
            if (!VectorUtil::contains(factions, faction)) {
                factions.push_back(faction);
            }
        }
    }
}