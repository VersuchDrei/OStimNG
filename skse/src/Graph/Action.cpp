#include "Action.h"

#include "GraphTable.h"

#include "Serial/Manager.h"
#include "Util/VectorUtil.h"

namespace Graph {
    bool ActionAttributes::hasTag(std::string tag) {
        return VectorUtil::contains(tags, tag);
    }


    float ActionAttributes::getActorStimulation(GameAPI::GameActor actor) {
        float stimulation = Serialization::getActionActorStimulation(actor.getBaseFormID(), type);
        return std::isnan(stimulation) ? this->actor.stimulation : stimulation;
    }

    float ActionAttributes::getActorMaxStimulation(GameAPI::GameActor actor) {
        float maxStimulation = Serialization::getActionActorMaxStimulation(actor.getBaseFormID(), type);
        return std::isnan(maxStimulation) ? this->actor.maxStimulation : maxStimulation;
    }

    float ActionAttributes::getTargetStimulation(GameAPI::GameActor actor) {
        float stimulation = Serialization::getActionTargetStimulation(actor.getBaseFormID(), type);
        return std::isnan(stimulation) ? target.stimulation : stimulation;
    }

    float ActionAttributes::getTargetMaxStimulation(GameAPI::GameActor actor) {
        float maxStimulation = Serialization::getActionTargetMaxStimulation(actor.getBaseFormID(), type);
        return std::isnan(maxStimulation) ? target.maxStimulation : maxStimulation;
    }

    float ActionAttributes::getPerformerStimulation(GameAPI::GameActor actor) {
        float stimulation = Serialization::getActionPerformerStimulation(actor.getBaseFormID(), type);
        return std::isnan(stimulation) ? performer.stimulation : stimulation;
    }

    float ActionAttributes::getPerformerMaxStimulation(GameAPI::GameActor actor) {
        float maxStimulation = Serialization::getActionPerformerMaxStimulation(actor.getBaseFormID(), type);
        return std::isnan(maxStimulation) ? performer.maxStimulation : maxStimulation;
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