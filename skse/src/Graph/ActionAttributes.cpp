#include "ActionAttributes.h"

#include "Serial/Manager.h"
#include "Util/VectorUtil.h"

namespace Graph {
    bool ActionAttributes::hasTag(std::string tag) { return VectorUtil::contains(tags, tag); }

    float ActionAttributes::getStimulation(Role role, GameAPI::GameActor actor) {
        float stimulation = Serialization::getActionStimulation(role, actor.getBaseFormID(), type);
        return std::isnan(stimulation) ? roles.get(role)->stimulation : stimulation;
    }

    float ActionAttributes::getMaxStimulation(Role role, GameAPI::GameActor actor) {
        float stimulation = Serialization::getActionMaxStimulation(role, actor.getBaseFormID(), type);
        return std::isnan(stimulation) ? roles.get(role)->maxStimulation : stimulation;
    }
}