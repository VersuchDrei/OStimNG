#include "../ThreadActor.h"

#include "../Thread.h"

namespace Threading {
    void ThreadActor::changeNodeAPIPre() {
        if (graphActor) {
            for (GameAPI::GameFaction faction : graphActor->factions) {
                faction.remove(actor);
            }
        }
    }

    void ThreadActor::changeNodeAPIPost() {
        for (GameAPI::GameFaction faction : graphActor->factions) {
            faction.add(actor);
        }

        for (GameAPI::GameFaction faction : graphActor->statFactions) {
            incrementStatFaction(faction);
        }


        if (!thread->isPlayerThread()) {
            return;
        }

        int playerIndex = thread->getPlayerIndex();

        for (Graph::Action::Action& action : thread->getCurrentNodeInternal()->actions) {
            if (action.roles.actor == playerIndex && action.roles.target == index) {
                for (GameAPI::GameFaction faction : action.attributes->roles.target.playerStatFactions) {
                    incrementStatFaction(faction);
                }
            } else if (action.roles.actor == index && action.roles.target == playerIndex) {
                for (GameAPI::GameFaction faction : action.attributes->roles.actor.playerStatFactions) {
                    incrementStatFaction(faction);
                }
            }
        }
    }

    void ThreadActor::freeAPI() {
        if (this->graphActor) {
            for (GameAPI::GameFaction faction : this->graphActor->factions) {
                faction.remove(actor);
            }
        }
    }


    void ThreadActor::incrementStatFaction(GameAPI::GameFaction faction) {
        if (statFactions.contains(faction)) {
            return;
        }

        statFactions.insert(faction);

        if (faction.contains(actor)) {
            int rank = faction.getRank(actor);
            if (rank <= 100) {
                faction.setRank(actor, rank + 1);
            }
        } else {
            faction.add(actor, 1);
        }
    }
}