#include "Graph/Node.h"

#include "Graph/LookupTable.h"
#include "Trait/Condition.h"
#include "Trait/TraitTable.h"
#include "Util/ActorUtil.h"
#include "Util/MCMTable.h"
#include "Util/StringUtil.h"
#include "Util/VectorUtil.h"
#include "SKEE.h"

namespace Graph {
    void Node::mergeActionRequirementsIntoActors() {
        for (Action* action : actions) {
            if (action->actor < actors.size()) {
                actors[action->actor]->requirements |= action->attributes->actor.requirements;
            }
            if (action->target < actors.size()) {
                actors[action->target]->requirements |= action->attributes->target.requirements;
            }
            if (action->performer < actors.size()) {
                actors[action->performer]->requirements |= action->attributes->performer.requirements;
            }
        }
    }

    bool Node::fulfilledBy(std::vector<Trait::ActorConditions> conditions) {
        int size = actors.size();
        if (size < conditions.size()) {
            return false;
        }

        for (int i = 0; i < size; i++) {
            if (!conditions[i].fulfills(actors[i]->conditions)) {
                return false;
            }
        }

        return true;
    }

    uint32_t Node::getStrippingMask(int position) {
        uint32_t mask = 0;
        for (auto& action : actions) {
            mask |= action->getStrippingMask(position);
        }
        return mask;
    }

    bool Node::doFullStrip(int position) {
        for (auto& action : actions) {
            if (action->doFullStrip(position)) {
                return true;
            }
        }
        return false;
    }

    std::string Node::getAutoTransitionForActor(int position, std::string type) {
        if (position < 0 || position >= actors.size()) {
            return "";
        }

        StringUtil::toLower(&type);
        auto iter = actors[position]->autotransitions.find(type);
        if (iter != actors[position]->autotransitions.end()) {
            return iter->second;
        }
        return "";
    }


    bool Node::hasActorTag(int position, std::string tag) {
        if (position < 0 || position >= actors.size()) {
            return false;
        }
        return VectorUtil::contains(actors[position]->tags, tag);
    }

    bool Node::hasAnyActorTag(int position, std::vector<std::string> tags) {
        if (position < 0 || position >= actors.size()) {
            return false;
        }
        return VectorUtil::containsAny(actors[position]->tags, tags);
    }

    bool Node::hasAllActorTags(int position, std::vector<std::string> tags) {
        if (position < 0 || position >= actors.size()) {
            return false;
        }
        return VectorUtil::containsAll(actors[position]->tags, tags);
    }

    bool Node::hasOnlyListedActorTags(int position, std::vector<std::string> tags) {
        if (position < 0 || position >= actors.size()) {
            return true;
        }
        return VectorUtil::containsAll(tags, actors[position]->tags);
    }

    int Node::findAction(std::function<bool(Action*)> condition) {
        size_t size = actions.size();
        for (int i = 0; i < size; i++) {
            if (condition(actions[i])) {
                return i;
            }
        }
        return -1;
    }

    std::vector<int> Node::findActions(std::function<bool(Action*)> condition) {
        std::vector<int> ret;
        size_t size = actions.size();
        for (int i = 0; i < size; i++) {
            if (condition(actions[i])) {
                ret.push_back(i);
            }
        }
        return ret;
    }

    bool Node::hasActionTag(std::string tag) {
        for (auto& action : actions) {
            if (action->attributes->hasTag(tag)) {
                return true;
            }
        }
        return false;
    }

    int Node::findAction(std::string type) {
        return findAction([type](Action* action) { return action->type == type; });
    }

    int Node::findAnyAction(std::vector<std::string> types) {
        return findAction([types](Action* action) { return VectorUtil::contains(types, action->type); });
    }

    int Node::findActionForActor(int position, std::string type) {
        return findAction([position, type](Action* action) {return action->actor == position && action->type == type;});
    }

    int Node::findAnyActionForActor(int position, std::vector<std::string> types) {
        return findAction([position, types](Action* action) {return action->actor == position && VectorUtil::contains(types, action->type);});
    }

    int Node::findActionForTarget(int position, std::string type) {
        return findAction([position, type](Action* action) {return action->target == position && action->type == type;});
    }

    int Node::findAnyActionForTarget(int position, std::vector<std::string> types) {
        return findAction([position, types](Action* action) {
            return action->target == position && VectorUtil::contains(types, action->type);
        });
    }

    int Node::findActionForActorAndTarget(int actorPosition, int targetPosition, std::string type) {
        return findAction([actorPosition, targetPosition, type](Action* action) {return action->actor == actorPosition && action->target == targetPosition && action->type == type;});
    }

    int Node::findAnyActionForActorAndTarget(int actorPosition, int targetPosition, std::vector<std::string> types) {
        return findAction([actorPosition, targetPosition, types](Action* action) {return action->actor == actorPosition && action->target == targetPosition && VectorUtil::contains(types, action->type);});
    }

    std::vector<Trait::FacialExpression*>* Node::getFacialExpressions(int position) {
        if (actors[position]->expressionAction != -1 && actors[position]->expressionAction < actions.size()) {
            auto& action = actions[actors[position]->expressionAction];
            if (action->target == position) {
                if (auto expressions = Trait::TraitTable::getExpressionsForActionTarget(action->type)) {
                    return expressions;
                }
            }
            if (action->actor == position) {
                if (auto expressions = Trait::TraitTable::getExpressionsForActionActor(action->type)) {
                    return expressions;
                }
            }
        }

        for (auto& action : actions) {
            if (action->target == position) {
                if (auto expressions = Trait::TraitTable::getExpressionsForActionTarget(action->type)) {
                    return expressions;
                }
            }
            if (action->actor == position) {
                if (auto expressions = Trait::TraitTable::getExpressionsForActionActor(action->type)) {
                    return expressions;
                }
            }
        }

        return Trait::TraitTable::getExpressionsForSet("default");
    }

    std::vector<Trait::FacialExpression*>* Node::getOverrideExpressions(int position) {
        if (hasActorTag(position, "openmouth") || findAnyActionForActor(position, {"blowjob", "cunnilingus", "suckingnipples"}) != -1) {
            return Trait::TraitTable::getExpressionsForSet("openmouth");
        }
        if (hasActorTag(position, "licking") || findAnyActionForActor(position, {"lickingnipples", "lickingpenis", "lickingtesticles", "lickingvagina", "rimjob"}) != -1) {
            return Trait::TraitTable::getExpressionsForSet("tongue");
        }
        return nullptr;
    }
}