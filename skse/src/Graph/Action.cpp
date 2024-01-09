#include "Action.h"

#include "GraphTable.h"

#include "Util/VectorUtil.h"

namespace Graph {
    float ActionActor::getStimulation(GameAPI::GameActor actor) {
        if (!stimulationFaction || !stimulationFaction.contains(actor)) {
            return stimulation;
        }
        return stimulationFaction.getRank(actor) * 0.05;
    }

    float ActionActor::getMaxStimulation(GameAPI::GameActor actor) {
        if (!maxStimulationFaction || !maxStimulationFaction.contains(actor)) {
            return maxStimulation;
        }
        return maxStimulationFaction.getRank(actor);
    }

    bool ActionAttributes::hasTag(std::string tag) {
        return VectorUtil::contains(tags, tag);
    }

    bool Action::doFullStrip(int position) {
        return actor == position && attributes->actor.fullStrip || 
              target == position && attributes->target.fullStrip || 
           performer == position && attributes->performer.fullStrip;
    }

    uint32_t Action::getStrippingMask(int position) {
        return (actor == position ? attributes->actor.strippingMask : 0) |
               (target == position ? attributes->target.strippingMask : 0) |
               (performer == position ? attributes->performer.strippingMask : 0);
    }

    bool Action::isType(std::string type) {
        return this->type == GraphTable::getActionAlias(type);
    }

    bool Action::isType(std::vector<std::string> types) {
        for (std::string& type : types) {
            if (isType(type)) {
                return true;
            }
        }
        return false;
    }
}