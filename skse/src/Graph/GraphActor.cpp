#include "GraphActor.h"

#include "Util/VectorUtil.h"

namespace Graph {
    void GraphActor::merge(ActionActor& actor) {
        condition.requirements |= actor.requirements;
        moan |= actor.moan;
        talk |= actor.talk;
        muffled |= actor.muffled;
        if (actor.faction && !VectorUtil::contains(factions, actor.faction)) {
            factions.push_back(actor.faction);
        }
    }
}