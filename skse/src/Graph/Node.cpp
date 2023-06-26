#include "Graph/Node.h"

#include "Graph/GraphTable.h"
#include "Trait/Condition.h"
#include "Trait/TraitTable.h"
#include "Util/ActorUtil.h"
#include "MCM/MCMTable.h"
#include "Util/StringUtil.h"
#include "Util/VectorUtil.h"

namespace Graph {
    void Node::mergeActionsIntoActors() {
        for (Action action : actions) {
            if (action.actor < actors.size()) {
                actors[action.actor].requirements |= action.attributes->actor.requirements;
                actors[action.actor].moan |= action.attributes->actor.moan;
                actors[action.actor].talk |= action.attributes->actor.talk;
                actors[action.actor].muffled |= action.attributes->actor.muffled;
            }
            if (action.target < actors.size()) {
                actors[action.target].requirements |= action.attributes->target.requirements;
                actors[action.target].moan |= action.attributes->target.moan;
                actors[action.target].talk |= action.attributes->target.talk;
                actors[action.target].muffled |= action.attributes->target.muffled;
            }
            if (action.performer < actors.size()) {
                actors[action.performer].requirements |= action.attributes->performer.requirements;
                actors[action.performer].moan |= action.attributes->performer.moan;
                actors[action.performer].talk |= action.attributes->performer.talk;
                actors[action.performer].muffled |= action.attributes->performer.muffled;
            }
        }
    }

    void Node::tryAddNavigation(std::string destination) {
        Node* navigationDestination = GraphTable::getNodeById(destination);
        if (!navigationDestination) {
            logger::warn("Couldn't add navigation from {} to {} because {} doesn't exist.", scene_id, destination, destination);
            return;
        }

        if (furnitureType != navigationDestination->furnitureType) {
            logger::warn("Couldn't add navigation from {} to {} because their furniture types don't match.", scene_id, destination);
            return;
        }

        if (actors.size() != navigationDestination->actors.size()) {
            logger::warn("Couldn't add navigation from {} to {} because their actor counts don't match.", scene_id, destination);
            return;
        }

        for (auto& navigation : navigations) {
            if (navigation.destination == navigationDestination) {
                return;
            }
        }

        navigations.push_back({.destination = navigationDestination});
    }

    bool Node::fulfilledBy(std::vector<Trait::ActorConditions> conditions) {
        int size = actors.size();
        if (size < conditions.size()) {
            return false;
        }

        for (int i = 0; i < size; i++) {
            if (!conditions[i].fulfills(actors[i].conditions)) {
                return false;
            }
        }

        return true;
    }

    Node* Node::getRandomNodeInRange(int distance, std::vector<Trait::ActorConditions> actorConditions, std::function<bool(Node*)> nodeCondition) {
        std::vector<Node*> nodes;
        std::vector<Node*> lastLevel = {this};
        std::vector<Node*> nextLevel;

        for (int i = 0; i < distance; i++) {
            for (Node* node : lastLevel) {
                for (Navigation nav : node->navigations) {
                    Node* dest = nav.destination;
                    if (dest->isTransition) {
                        if (dest->navigations.empty()) {
                            continue;
                        } else {
                            dest = dest->navigations[0].destination;
                        }
                    }
                    if (!VectorUtil::contains(nodes, dest) && dest->fulfilledBy(actorConditions)) {
                        nodes.push_back(dest);
                        nextLevel.push_back(dest);
                    }
                }
            }

            if (nextLevel.empty()) {
                break;
            }

            lastLevel = nextLevel;
            nextLevel.clear();
        }

        for (auto& node : nodes) {
            if (!node->noRandomSelection && nodeCondition(node)) {
                return node;
            }
        }

        return nullptr;
    }

    uint32_t Node::getStrippingMask(int position) {
        uint32_t mask = 0;
        for (auto& action : actions) {
            mask |= action.getStrippingMask(position);
        }
        return mask;
    }

    bool Node::doFullStrip(int position) {
        for (auto& action : actions) {
            if (action.doFullStrip(position)) {
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
        auto iter = actors[position].autotransitions.find(type);
        if (iter != actors[position].autotransitions.end()) {
            return iter->second;
        }
        return "";
    }


    bool Node::hasActorTag(int position, std::string tag) {
        if (position < 0 || position >= actors.size()) {
            return false;
        }
        return VectorUtil::contains(actors[position].tags, tag);
    }

    bool Node::hasAnyActorTag(int position, std::vector<std::string> tags) {
        if (position < 0 || position >= actors.size()) {
            return false;
        }
        return VectorUtil::containsAny(actors[position].tags, tags);
    }

    bool Node::hasAllActorTags(int position, std::vector<std::string> tags) {
        if (position < 0 || position >= actors.size()) {
            return false;
        }
        return VectorUtil::containsAll(actors[position].tags, tags);
    }

    bool Node::hasOnlyListedActorTags(int position, std::vector<std::string> tags) {
        if (position < 0 || position >= actors.size()) {
            return true;
        }
        return VectorUtil::containsAll(tags, actors[position].tags);
    }

    int Node::findAction(std::function<bool(Action)> condition) {
        size_t size = actions.size();
        for (int i = 0; i < size; i++) {
            if (condition(actions[i])) {
                return i;
            }
        }
        return -1;
    }

    std::vector<int> Node::findActions(std::function<bool(Action)> condition) {
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
            if (action.attributes->hasTag(tag)) {
                return true;
            }
        }
        return false;
    }

    int Node::findAction(std::string type) {
        return findAction([type](Action action) { return action.type == type; });
    }

    int Node::findAnyAction(std::vector<std::string> types) {
        return findAction([types](Action action) { return VectorUtil::contains(types, action.type); });
    }

    int Node::findActionForActor(int position, std::string type) {
        return findAction([position, type](Action action) {return action.actor == position && action.type == type;});
    }

    int Node::findAnyActionForActor(int position, std::vector<std::string> types) {
        return findAction([position, types](Action action) {return action.actor == position && VectorUtil::contains(types, action.type);});
    }

    int Node::findActionForTarget(int position, std::string type) {
        return findAction([position, type](Action action) {return action.target == position && action.type == type;});
    }

    int Node::findAnyActionForTarget(int position, std::vector<std::string> types) {
        return findAction([position, types](Action action) {return action.target == position && VectorUtil::contains(types, action.type);});
    }

    int Node::findActionForActorAndTarget(int actorPosition, int targetPosition, std::string type) {
        return findAction([actorPosition, targetPosition, type](Action action) {return action.actor == actorPosition && action.target == targetPosition && action.type == type;});
    }

    int Node::findAnyActionForActorAndTarget(int actorPosition, int targetPosition, std::vector<std::string> types) {
        return findAction([actorPosition, targetPosition, types](Action action) {return action.actor == actorPosition && action.target == targetPosition && VectorUtil::contains(types, action.type);});
    }

    std::vector<Trait::FacialExpression*>* Node::getFacialExpressions(int position) {
        if (actors[position].expressionAction != -1 && actors[position].expressionAction < actions.size()) {
            auto& action = actions[actors[position].expressionAction];
            if (action.target == position) {
                if (auto expressions = Trait::TraitTable::getExpressionsForActionTarget(action.type)) {
                    return expressions;
                }
            }
            if (action.actor == position) {
                if (auto expressions = Trait::TraitTable::getExpressionsForActionActor(action.type)) {
                    return expressions;
                }
            }
        }

        for (auto& action : actions) {
            if (action.target == position) {
                if (auto expressions = Trait::TraitTable::getExpressionsForActionTarget(action.type)) {
                    return expressions;
                }
            }
            if (action.actor == position) {
                if (auto expressions = Trait::TraitTable::getExpressionsForActionActor(action.type)) {
                    return expressions;
                }
            }
        }

        return Trait::TraitTable::getExpressionsForSet("default");
    }

    std::vector<Trait::FacialExpression*>* Node::getOverrideExpressions(int position) {
        if (actors.size() <= position) {
            return nullptr;
        }

        std::vector<Trait::FacialExpression*>* expression;
        if (!actors[position].expressionOverride.empty()) {
            expression = Trait::TraitTable::getExpressionsForSet(actors[position].expressionOverride);
            if (expression) {
                return expression;
            }
        }

        for (Action action : actions) {
            if (action.actor == position && !action.attributes->actor.expressionOverride.empty()) {
                expression = Trait::TraitTable::getExpressionsForSet(action.attributes->actor.expressionOverride);
                if (expression) {
                    return expression;
                }
            }
            if (action.target == position && !action.attributes->target.expressionOverride.empty()) {
                expression = Trait::TraitTable::getExpressionsForSet(action.attributes->target.expressionOverride);
                if (expression) {
                    return expression;
                }
            }
            if (action.performer == position && !action.attributes->performer.expressionOverride.empty()) {
                expression = Trait::TraitTable::getExpressionsForSet(action.attributes->performer.expressionOverride);
                if (expression) {
                    return expression;
                }
            }
        }

        return nullptr;
    }
}