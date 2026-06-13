#include "ActionActor.h"

#include "Util/VectorUtil.h"

namespace Graph {
    namespace Action {
        void ActionActor::merge(ActionActor& other) {
            if (other.fullStrip) {
                fullStrip = true;
            }

            if (other.moan) {
                moan = true;
            }

            if (other.talk) {
                talk = true;
            }

            if (other.muffled) {
                muffled = true;
            }

            strippingMask |= other.strippingMask;

            for (GameAPI::GameFaction faction : other.factions) {
                if (!VectorUtil::contains(factions, faction)) {
                    factions.push_back(faction);
                }
            }

            for (GameAPI::GameFaction faction : other.statFactions) {
                if (!VectorUtil::contains(statFactions, faction)) {
                    statFactions.push_back(faction);
                }
            }

            for (GameAPI::GameFaction faction : other.playerStatFactions) {
                if (!VectorUtil::contains(playerStatFactions, faction)) {
                    playerStatFactions.push_back(faction);
                }
            }

            for (std::string& slot : other.toySlots) {
                if (!VectorUtil::contains(toySlots, slot)) {
                    toySlots.push_back(slot);
                }
            }
        }
    }
}