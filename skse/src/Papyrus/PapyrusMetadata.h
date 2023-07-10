#pragma once

#include "Graph/GraphTable.h"
#include "Util/MapUtil.h"
#include "Util/StringUtil.h"
#include "Util/VectorUtil.h"
#include "Util.h"

namespace PapyrusMetadata {
    using VM = RE::BSScript::IVirtualMachine;

    int findAction(std::string id, std::function<bool(Graph::Action)> condition) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            size_t size = node->actions.size();
            for (int i = 0; i < size; i++) {
                if (condition(node->actions[i])) {
                    return i;
                }
            }
        }
        return -1;
    }

    std::vector<int> findActions(std::string id, std::function<bool(Graph::Action)> condition) {
        std::vector<int> ret;
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            size_t size = node->actions.size();
            for (int i = 0; i < size; i++) {
                if (condition(node->actions[i])) {
                    ret.push_back(i);
                }
            }
        }
        return ret;
    }

    // *********************************************************
    // helper functions specifically for superload
    // *********************************************************
    bool checkConditions(std::vector<std::function<bool(Graph::Action)>> &conditions, Graph::Action action) {
        for (std::function<bool(Graph::Action)> condition : conditions) {
            if (!condition(action)) {
                return false;
            }
        }
        return true;
    }

    // *********************************************************
    // start of papyrus bound functions
    // *********************************************************

#pragma region general
    bool IsTransition(RE::StaticFunctionTag*, std::string id) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return node->isTransition;
        }
        return false;
    }

    int GetDefaultSpeed(RE::StaticFunctionTag*, std::string id) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return node->defaultSpeed;
        }
        return 0;
    }

    int GetMaxSpeed(RE::StaticFunctionTag*, std::string id) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return node->speeds.size() - 1;
        }
        return 0;
    }

    int GetActorCount(RE::StaticFunctionTag*, std::string id) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return node->actors.size();
        }
        return 0;
    }

    std::string GetAnimationId(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (node->speeds.size() > index) {
                return node->speeds[index].animation;
            }
        }
        return "";
    }

    std::string GetAutoTransitionForActor(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return node->getAutoTransitionForActor(position, type);
        }
        return "";
    }
#pragma endregion

#pragma region tags
#pragma region scene_tags
    std::vector<std::string> GetSceneTags(RE::StaticFunctionTag*, std::string id) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return node->tags;
        }
        return std::vector<std::string>();
    }

    bool HasSceneTag(RE::StaticFunctionTag*, std::string id, std::string tag) {
        StringUtil::toLower(&tag);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return VectorUtil::contains(node->tags, tag);
        }
        return false;
    }

    bool HasAnySceneTag(RE::StaticFunctionTag*, std::string id, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return VectorUtil::containsAny(node->tags, tags);
        }
        return false;
    }

    bool HasAnySceneTagCSV(RE::StaticFunctionTag* sft, std::string id, std::string tags) {
        return HasAnySceneTag(sft, id, StringUtil::toTagVector(tags));
    }

    bool HasAllSceneTags(RE::StaticFunctionTag*, std::string id, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return VectorUtil::containsAll(node->tags, tags);
        }
        return false;
    }

    bool HasAllSceneTagsCSV(RE::StaticFunctionTag* sft, std::string id, std::string tags) {
        return HasAllSceneTags(sft, id, StringUtil::toTagVector(tags));
    }

    std::vector<std::string> GetSceneTagOverlap(RE::StaticFunctionTag*, std::string id, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return VectorUtil::getOverlap(node->tags, tags);
        }

        return {};
    }

    std::vector<std::string> GetSceneTagOverlapCSV(RE::StaticFunctionTag* sft, std::string id, std::string tags) {
        return GetSceneTagOverlap(sft, id, StringUtil::toTagVector(tags));
    }
#pragma endregion

#pragma region actor_tags
    std::vector<std::string> GetActorTags(RE::StaticFunctionTag*, std::string id, int position) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (node->actors.size() > position) {
                return node->actors[position].tags;
            }
        }
        return std::vector<std::string>();
    }

    bool HasActorTag(RE::StaticFunctionTag*, std::string id, int position, std::string tag) {
        StringUtil::toLower(&tag);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (node->actors.size() > position) {
                return node->hasActorTag(position, tag);
            }
        }
        return false;
    }

    bool HasAnyActorTag(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (node->actors.size() > position) {
                return VectorUtil::containsAny(node->actors[position].tags, tags);
            }
        }
        return false;
    }

    bool HasAnyActorTagCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string tags) {
        return HasAnyActorTag(sft, id, position, StringUtil::toTagVector(tags));
    }

    bool HasAllActorTags(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (node->actors.size() > position) {
                return VectorUtil::containsAll(node->actors[position].tags, tags);
            }
        }
        return false;
    }

    bool HasAllActorTagsCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string tags) {
        return HasAllActorTags(sft, id, position, StringUtil::toTagVector(tags));
    }

    std::vector<std::string> GetActorTagOverlap(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (node->actors.size() > position) {
                return VectorUtil::getOverlap(node->actors[position].tags, tags);
            }
            
        }

        return {};
    }

    std::vector<std::string> GetActorTagOverlapCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string tags) {
        return GetActorTagOverlap(sft, id, position, StringUtil::toTagVector(tags));
    }
#pragma endregion
#pragma endregion

#pragma region actions
#pragma region find_action
#pragma region actions_general
    bool HasActions(RE::StaticFunctionTag*, std::string id) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            return !node->actions.empty();
        }
        return false;
    }

    int FindAction(RE::StaticFunctionTag*, std::string id, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [type](Graph::Action action) { return action.type == type; });
    }

    int FindAnyAction(RE::StaticFunctionTag*, std::string id, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [types](Graph::Action action) { return VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionCSV(RE::StaticFunctionTag* sft, std::string id, std::string types) {
        return FindAnyAction(sft, id, StringUtil::toTagVector(types));
    }

    std::vector<int> FindActions(RE::StaticFunctionTag*, std::string id, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [type](Graph::Action action) { return action.type == type; });
    }

    std::vector<int> FindAllActions(RE::StaticFunctionTag*, std::string id, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [types](Graph::Action action) { return VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsCSV(RE::StaticFunctionTag* sft, std::string id, std::string types) {
        return FindAllActions(sft, id, StringUtil::toTagVector(types));
    }
#pragma endregion

#pragma region actions_by_actor
    int FindActionForActor(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [position, type](Graph::Action action) { return action.actor == position && action.type == type; });
    }

    int FindAnyActionForActor(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [position, types](Graph::Action action) { return action.actor == position && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForActorCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAnyActionForActor(sft, id, position, StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForActor(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [position, type](Graph::Action action) { return action.actor == position && action.type == type; });
    }

    std::vector<int> FindAllActionsForActor(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [position, types](Graph::Action action) { return action.actor == position && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForActorCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAllActionsForActor(sft, id, position, StringUtil::toTagVector(types));
    }

    int FindActionForActors(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [positions, type](Graph::Action action) { return VectorUtil::contains(positions, action.actor) && action.type == type; });
    }

    int FindActionForActorsCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionForActors(sft, id, VectorUtil::stoiv(positions), type);
    }

    int FindAnyActionForActors(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [positions, types](Graph::Action action) { return VectorUtil::contains(positions, action.actor) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForActorsCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAnyActionForActors(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForActors(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [positions, type](Graph::Action action) { return VectorUtil::contains(positions, action.actor) && action.type == type; });
    }

    std::vector<int> FindActionsForActorsCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionsForActors(sft, id, VectorUtil::stoiv(positions), type);
    }

    std::vector<int> FindAllActionsForActors(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [positions, types](Graph::Action action) { return VectorUtil::contains(positions, action.actor) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForActorsCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAllActionsForActors(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }
#pragma endregion

#pragma region actions_by_target
    int FindActionForTarget(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [position, type](Graph::Action action) { return action.target == position && action.type == type; });
    }

    int FindAnyActionForTarget(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [position, types](Graph::Action action) { return action.target == position && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForTargetCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAnyActionForTarget(sft, id, position, StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForTarget(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [position, type](Graph::Action action) { return action.target == position && action.type == type; });
    }

    std::vector<int> FindAllActionsForTarget(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [position, types](Graph::Action action) { return action.target == position && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForTargetCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAllActionsForTarget(sft, id, position, StringUtil::toTagVector(types));
    }

    int FindActionForTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [positions, type](Graph::Action action) { return VectorUtil::contains(positions, action.target) && action.type == type; });
    }

    int FindActionForTargetsCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionForTargets(sft, id, VectorUtil::stoiv(positions), type);
    }

    int FindAnyActionForTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [positions, types](Graph::Action action) { return VectorUtil::contains(positions, action.target) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForTargetsCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAnyActionForTargets(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [positions, type](Graph::Action action) { return VectorUtil::contains(positions, action.target) && action.type == type; });
    }

    std::vector<int> FindActionsForTargetsCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionsForTargets(sft, id, VectorUtil::stoiv(positions), type);
    }

    std::vector<int> FindAllActionsForTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [positions, types](Graph::Action action) { return VectorUtil::contains(positions, action.target) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForTargetsCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAllActionsForTargets(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }
#pragma endregion

#pragma region actions_by_performer
    int FindActionForPerformer(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [position, type](Graph::Action action) { return action.performer == position && action.type == type; });
    }

    int FindAnyActionForPerformer(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [position, types](Graph::Action action) { return action.performer == position && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForPerformerCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAnyActionForPerformer(sft, id, position, StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForPerformer(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [position, type](Graph::Action action) { return action.performer == position && action.type == type; });
    }

    std::vector<int> FindAllActionsForPerformer(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [position, types](Graph::Action action) { return action.performer == position && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForPerformerCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAllActionsForPerformer(sft, id, position, StringUtil::toTagVector(types));
    }

    int FindActionForPerformers(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [positions, type](Graph::Action action) { return VectorUtil::contains(positions, action.performer) && action.type == type; });
    }

    int FindActionForPerformersCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionForPerformers(sft, id, VectorUtil::stoiv(positions), type);
    }

    int FindAnyActionForPerformers(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [positions, types](Graph::Action action) { return VectorUtil::contains(positions, action.performer) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForPerformersCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAnyActionForPerformers(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForPerformers(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [positions, type](Graph::Action action) { return VectorUtil::contains(positions, action.performer) && action.type == type; });
    }

    std::vector<int> FindActionsForPerformersCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionsForPerformers(sft, id, VectorUtil::stoiv(positions), type);
    }

    std::vector<int> FindAllActionsForPerformers(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [positions, types](Graph::Action action) { return VectorUtil::contains(positions, action.performer) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForPerformersCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAllActionsForPerformers(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }
#pragma endregion
    
#pragma region actions_by_actor_and_target
    int FindActionForActorAndTarget(RE::StaticFunctionTag*, std::string id, int actorPosition, int targetPosition, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [actorPosition, targetPosition, type](Graph::Action action) { return action.actor == actorPosition && action.target == targetPosition && action.type == type; });
    }

    int FindAnyActionForActorAndTarget(RE::StaticFunctionTag*, std::string id, int actorPosition, int targetPosition, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [actorPosition, targetPosition, types](Graph::Action action) { return action.actor == actorPosition && action.target == targetPosition && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForActorAndTargetCSV(RE::StaticFunctionTag* sft, std::string id, int actorPosition, int targetPosition, std::string types) {
        return FindAnyActionForActorAndTarget(sft, id, actorPosition, targetPosition, StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForActorAndTarget(RE::StaticFunctionTag*, std::string id, int actorPosition, int targetPosition, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [actorPosition, targetPosition, type](Graph::Action action) { return action.actor == actorPosition && action.target == targetPosition && action.type == type; });
    }

    std::vector<int> FindAllActionsForActorAndTarget(RE::StaticFunctionTag*, std::string id, int actorPosition, int targetPosition, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [actorPosition, targetPosition, types](Graph::Action action) { return action.actor == actorPosition && action.target == targetPosition && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForActorAndTargetCSV(RE::StaticFunctionTag* sft, std::string id, int actorPosition, int targetPosition, std::string types) {
        return FindAllActionsForActorAndTarget(sft, id, actorPosition, targetPosition, StringUtil::toTagVector(types));
    }

    int FindActionForActorsAndTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> actorPositions, std::vector<int> targetPositions, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [actorPositions, targetPositions, type](Graph::Action action) { return VectorUtil::contains(actorPositions, action.actor) && VectorUtil::contains(targetPositions, action.target) && action.type == type; });
    }

    int FindActionForActorsAndTargetsCSV(RE::StaticFunctionTag* sft, std::string id, std::string actorPositions, std::string targetPositions, std::string type) {
        return FindActionForActorsAndTargets(sft, id, VectorUtil::stoiv(actorPositions), VectorUtil::stoiv(targetPositions), type);
    }

    int FindAnyActionForActorsAndTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> actorPositions, std::vector<int> targetPositions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [actorPositions, targetPositions, types](Graph::Action action) { return VectorUtil::contains(actorPositions, action.actor) && VectorUtil::contains(targetPositions, action.target) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForActorsAndTargetsCSV(RE::StaticFunctionTag* sft, std::string id, std::string actorPositions, std::string targetPositions, std::string types) {
        return FindAnyActionForActorsAndTargets(sft, id, VectorUtil::stoiv(actorPositions), VectorUtil::stoiv(targetPositions), StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForActorsAndTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> actorPositions, std::vector<int> targetPositions, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [actorPositions, targetPositions, type](Graph::Action action) { return VectorUtil::contains(actorPositions, action.actor) && VectorUtil::contains(targetPositions, action.target) && action.type == type; });
    }

    std::vector<int> FindActionsForActorsAndTargetsCSV(RE::StaticFunctionTag* sft, std::string id, std::string actorPositions, std::string targetPositions, std::string type) {
        return FindActionsForActorsAndTargets(sft, id, VectorUtil::stoiv(actorPositions), VectorUtil::stoiv(targetPositions), type);
    }

    std::vector<int> FindAllActionsForActorsAndTargets(RE::StaticFunctionTag*, std::string id, std::vector<int> actorPositions, std::vector<int> targetPositions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [actorPositions, targetPositions, types](Graph::Action action) { return VectorUtil::contains(actorPositions, action.actor) && VectorUtil::contains(targetPositions, action.target) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForActorsAndTargetsCSV(RE::StaticFunctionTag* sft, std::string id, std::string actorPositions, std::string targetPositions, std::string types) {
        return FindAllActionsForActorsAndTargets(sft, id, VectorUtil::stoiv(actorPositions), VectorUtil::stoiv(targetPositions), StringUtil::toTagVector(types));
    }
#pragma endregion

#pragma region actions_by_mate
    int FindActionForMate(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [position, type](Graph::Action action) { return (action.actor == position || action.target == position) && action.type == type; });
    }

    int FindAnyActionForMate(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [position, types](Graph::Action action) { return (action.actor == position || action.target == position) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForMateCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAnyActionForMate(sft, id, position, StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForMate(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [position, type](Graph::Action action) { return (action.actor == position || action.target == position) && action.type == type; });
    }

    std::vector<int> FindAllActionsForMate(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [position, types](Graph::Action action) { return (action.actor == position || action.target == position) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForMateCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAllActionsForMate(sft, id, position, StringUtil::toTagVector(types));
    }

    int FindActionForMatesAny(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [positions, type](Graph::Action action) { return VectorUtil::containsAny(positions, {action.actor, action.target}) && action.type == type; });
    }

    int FindActionForMatesAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionForMatesAny(sft, id, VectorUtil::stoiv(positions), type);
    }

    int FindAnyActionForMatesAny(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [positions, types](Graph::Action action) { return VectorUtil::containsAny(positions, {action.actor, action.target}) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForMatesAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAnyActionForMatesAny(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForMatesAny(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [positions, type](Graph::Action action) { return VectorUtil::containsAny(positions, {action.actor, action.target}) && action.type == type; });
    }

    std::vector<int> FindActionsForMatesAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionsForMatesAny(sft, id, VectorUtil::stoiv(positions), type);
    }

    std::vector<int> FindAllActionsForMatesAny(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [positions, types](Graph::Action action) { return VectorUtil::containsAny(positions, {action.actor, action.target}) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForMatesAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAllActionsForMatesAny(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }

    int FindActionForMatesAll(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [positions, type](Graph::Action action) { return VectorUtil::containsAll(positions, {action.actor, action.target}) && action.type == type; });
    }

    int FindActionForMatesAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionForMatesAll(sft, id, VectorUtil::stoiv(positions), type);
    }

    int FindAnyActionForMatesAll(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [positions, types](Graph::Action action) { return VectorUtil::containsAll(positions, {action.actor, action.target}) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForMatesAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAnyActionForMatesAll(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForMatesAll(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [positions, type](Graph::Action action) { return VectorUtil::containsAll(positions, {action.actor, action.target}) && action.type == type; });
    }

    std::vector<int> FindActionsForMatesAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionsForMatesAll(sft, id, VectorUtil::stoiv(positions), type);
    }

    std::vector<int> FindAllActionsForMatesAll(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [positions, types](Graph::Action action) { return VectorUtil::containsAll(positions, {action.actor, action.target}) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForMatesAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAllActionsForMatesAll(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }
#pragma endregion

#pragma region actions_by_participant
    int FindActionForParticipant(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [position, type](Graph::Action action) { return (action.actor == position || action.target == position || action.performer == position) && action.type == type; });
    }

    int FindAnyActionForParticipant(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [position, types](Graph::Action action) { return (action.actor == position || action.target == position || action.performer == position) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForParticipantCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAnyActionForParticipant(sft, id, position, StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForParticipant(RE::StaticFunctionTag*, std::string id, int position, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [position, type](Graph::Action action) { return (action.actor == position || action.target == position || action.performer == position) && action.type == type; });
    }

    std::vector<int> FindAllActionsForParticipant(RE::StaticFunctionTag*, std::string id, int position, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [position, types](Graph::Action action) { return (action.actor == position || action.target == position || action.performer == position) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForParticipantCSV(RE::StaticFunctionTag* sft, std::string id, int position, std::string types) {
        return FindAllActionsForParticipant(sft, id, position, StringUtil::toTagVector(types));
    }

    int FindActionForParticipantsAny(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [positions, type](Graph::Action action) { return VectorUtil::containsAny(positions, {action.actor, action.target, action.performer}) && action.type == type; });
    }

    int FindActionForParticipantsAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionForParticipantsAny(sft, id, VectorUtil::stoiv(positions), type);
    }

    int FindAnyActionForParticipantsAny(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [positions, types](Graph::Action action) { return VectorUtil::containsAny(positions, {action.actor, action.target, action.performer}) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForParticipantsAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAnyActionForParticipantsAny(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForParticipantsAny(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [positions, type](Graph::Action action) { return VectorUtil::containsAny(positions, {action.actor, action.target, action.performer}) && action.type == type; });
    }

    std::vector<int> FindActionsForParticipantsAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionsForParticipantsAny(sft, id, VectorUtil::stoiv(positions), type);
    }

    std::vector<int> FindAllActionsForParticipantsAny(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [positions, types](Graph::Action action) { return VectorUtil::containsAny(positions, {action.actor, action.target, action.performer}) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForParticipantsAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAllActionsForParticipantsAny(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }

    int FindActionForParticipantsAll(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findAction(id, [positions, type](Graph::Action action) { return VectorUtil::containsAll(positions, {action.actor, action.target, action.performer}) && action.type == type; });
    }

    int FindActionForParticipantsAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionForParticipantsAll(sft, id, VectorUtil::stoiv(positions), type);
    }

    int FindAnyActionForParticipantsAll(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findAction(id, [positions, types](Graph::Action action) { return VectorUtil::containsAll(positions, {action.actor, action.target, action.performer}) && VectorUtil::contains(types, action.type); });
    }

    int FindAnyActionForParticipantsAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAnyActionForParticipantsAll(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }

    std::vector<int> FindActionsForParticipantsAll(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::string type) {
        StringUtil::toLower(&type);
        return findActions(id, [positions, type](Graph::Action action) { return VectorUtil::containsAll(positions, {action.actor, action.target, action.performer}) && action.type == type; });
    }

    std::vector<int> FindActionsForParticipantsAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string type) {
        return FindActionsForParticipantsAll(sft, id, VectorUtil::stoiv(positions), type);
    }

    std::vector<int> FindAllActionsForParticipantsAll(RE::StaticFunctionTag*, std::string id, std::vector<int> positions, std::vector<std::string> types) {
        StringUtil::toLower(&types);
        return findActions(id, [positions, types](Graph::Action action) { return VectorUtil::containsAll(positions, {action.actor, action.target, action.performer}) && VectorUtil::contains(types, action.type); });
    }

    std::vector<int> FindAllActionsForParticipantsAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string positions, std::string types) {
        return FindAllActionsForParticipantsAll(sft, id, VectorUtil::stoiv(positions), StringUtil::toTagVector(types));
    }
#pragma endregion

#pragma region action_superload
    int FindActionSuperloadCSVv2(RE::StaticFunctionTag*, std::string id, std::string actorPositions, std::string targetPositions, std::string performerPositions, std::string matePositionsAny, std::string matePositionsAll, std::string participantPositionsAny, std::string participantPositionsAll, std::string types, std::string anyActionTag, std::string allActionTags, std::string actionTagWhitelist, std::string actionTagBlacklist, std::string anyCustomIntRecord, std::string allCustomIntRecords, std::string anyCustomFloatRecord, std::string allCustomFloatRecords, std::string anyCustomStringRecord, std::string allCustomStringRecords, std::string anyCustomIntListRecord, std::string allCustomIntListRecords, std::string anyCustomFloatListRecord, std::string allCustomFloatListRecords, std::string anyCustomStringListRecord, std::string allCustomStringListRecords) {
        std::vector<std::function<bool(Graph::Action)>> conditions;
        

        if (!actorPositions.empty()) {
            std::vector<int> actorPos = VectorUtil::stoiv(actorPositions);
            conditions.push_back([actorPos](Graph::Action action){return VectorUtil::contains(actorPos, action.actor);});
        }

        if (!targetPositions.empty()) {
            std::vector<int> targetPos = VectorUtil::stoiv(targetPositions);
            conditions.push_back([targetPos](Graph::Action action){return VectorUtil::contains(targetPos, action.target);});
        }

        if (!performerPositions.empty()) {
            std::vector<int> performerPos = VectorUtil::stoiv(performerPositions);
            conditions.push_back([performerPos](Graph::Action action){return VectorUtil::contains(performerPos, action.performer);});
        }

        if (!matePositionsAny.empty()) {
            std::vector<int> matePosAny = VectorUtil::stoiv(matePositionsAny);
            conditions.push_back([matePosAny](Graph::Action action){return VectorUtil::containsAny(matePosAny, {action.actor, action.target});});
        }

        if (!matePositionsAll.empty()) {
            std::vector<int> matePosAll = VectorUtil::stoiv(matePositionsAll);
            conditions.push_back([matePosAll](Graph::Action action){return VectorUtil::containsAll(matePosAll, {action.actor, action.target});});
        }

        if (!participantPositionsAny.empty()) {
            std::vector<int> participantPosAny = VectorUtil::stoiv(participantPositionsAny);
            conditions.push_back([participantPosAny](Graph::Action action){return VectorUtil::containsAny(participantPosAny, {action.actor, action.target, action.performer});});
        }

        if (!participantPositionsAll.empty()) {
            std::vector<int> participantPosAll = VectorUtil::stoiv(participantPositionsAll);
            conditions.push_back([participantPosAll](Graph::Action action){return VectorUtil::containsAll(participantPosAll, {action.actor, action.target, action.performer});});
        }

        if (!types.empty()) {
            std::vector<std::string> typesVector = StringUtil::toTagVector(types);
            conditions.push_back([typesVector](Graph::Action action){return VectorUtil::contains(typesVector, action.type);});
        }


        if (!anyActionTag.empty()) {
            std::vector<std::string> AnyActionTagVector = StringUtil::toTagVector(anyActionTag);
            conditions.push_back([AnyActionTagVector](Graph::Action action){return VectorUtil::containsAny(action.attributes->tags, AnyActionTagVector);});
        }

        if (!allActionTags.empty()) {
            std::vector<std::string> AllActionTagsVector = StringUtil::toTagVector(allActionTags);
            conditions.push_back([AllActionTagsVector](Graph::Action action){return VectorUtil::containsAll(action.attributes->tags, AllActionTagsVector);});
        }

        if (!actionTagWhitelist.empty()) {
            std::vector<std::string> actionTagWhitelistVector = StringUtil::toTagVector(actionTagWhitelist);
            conditions.push_back([actionTagWhitelistVector](Graph::Action action){return VectorUtil::containsAll(actionTagWhitelistVector, action.attributes->tags);});
        }

        if (!actionTagBlacklist.empty()) {
            std::vector<std::string> actionTagBlacklistVector = StringUtil::toTagVector(actionTagBlacklist);
            conditions.push_back([actionTagBlacklistVector](Graph::Action action){return !VectorUtil::containsAny(actionTagBlacklistVector, action.attributes->tags);});
        }


        if (!anyCustomIntRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomIntRecordMatrix = StringUtil::toTagMatrix(anyCustomIntRecord);

            std::vector<std::string> actorKeys = anyCustomIntRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.ints, actorKeys);});
            }

            if (anyCustomIntRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomIntRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.ints, targetKeys);});
                }
            }

            if (anyCustomIntRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomIntRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.ints, performerKeys);});
                }
            }
        }

        if (!allCustomIntRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomIntRecordsMatrix = StringUtil::toTagMatrix(allCustomIntRecords);

            std::vector<std::string> actorKeys = allCustomIntRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.ints, actorKeys);});
            }

            if (allCustomIntRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomIntRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.ints, targetKeys);});
                }
            }

            if (allCustomIntRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomIntRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.ints, performerKeys);});
                }
            }
        }

        if (!anyCustomFloatRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomFloatRecordMatrix = StringUtil::toTagMatrix(anyCustomFloatRecord);

            std::vector<std::string> actorKeys = anyCustomFloatRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.floats, actorKeys);});
            }

            if (anyCustomFloatRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomFloatRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.floats, targetKeys);});
                }
            }

            if (anyCustomFloatRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomFloatRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.floats, performerKeys);});
                }
            }
        }

        if (!allCustomFloatRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomFloatRecordsMatrix = StringUtil::toTagMatrix(allCustomFloatRecords);

            std::vector<std::string> actorKeys = allCustomFloatRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.floats, actorKeys);});
            }

            if (allCustomFloatRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomFloatRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.floats, targetKeys);});
                }
            }

            if (allCustomFloatRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomFloatRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.floats, performerKeys);});
                }
            }
        }

        if (!anyCustomStringRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomStringRecordMatrix = StringUtil::toTagMatrix(anyCustomStringRecord);

            std::vector<std::string> actorKeys = anyCustomStringRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.strings, actorKeys);});
            }

            if (anyCustomStringRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomStringRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.strings, targetKeys);});
                }
            }

            if (anyCustomStringRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomStringRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.strings, performerKeys);});
                }
            }
        }

        if (!allCustomStringRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomStringRecordsMatrix = StringUtil::toTagMatrix(allCustomStringRecords);

            std::vector<std::string> actorKeys = allCustomStringRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.strings, actorKeys);});
            }

            if (allCustomStringRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomStringRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.strings, targetKeys);});
                }
            }

            if (allCustomStringRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomStringRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.strings, performerKeys);});
                }
            }
        }

        if (!anyCustomIntListRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomIntListRecordMatrix = StringUtil::toTagMatrix(anyCustomIntListRecord);

            std::vector<std::string> actorKeys = anyCustomIntListRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.intLists, actorKeys);});
            }

            if (anyCustomIntListRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomIntListRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.intLists, targetKeys);});
                }
            }

            if (anyCustomIntListRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomIntListRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.intLists, performerKeys);});
                }
            }
        }

        if (!allCustomIntListRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomIntListRecordsMatrix = StringUtil::toTagMatrix(allCustomIntListRecords);

            std::vector<std::string> actorKeys = allCustomIntListRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.intLists, actorKeys);});
            }

            if (allCustomIntListRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomIntListRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.intLists, targetKeys);});
                }
            }

            if (allCustomIntListRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomIntListRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.intLists, performerKeys);});
                }
            }
        }

        if (!anyCustomFloatListRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomFloatListRecordMatrix = StringUtil::toTagMatrix(anyCustomFloatListRecord);

            std::vector<std::string> actorKeys = anyCustomFloatListRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.floatLists, actorKeys);});
            }

            if (anyCustomFloatListRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomFloatListRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.floatLists, targetKeys);});
                }
            }

            if (anyCustomFloatListRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomFloatListRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.floatLists, performerKeys);});
                }
            }
        }

        if (!allCustomFloatListRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomFloatListRecordsMatrix = StringUtil::toTagMatrix(allCustomFloatListRecords);

            std::vector<std::string> actorKeys = allCustomFloatListRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.floatLists, actorKeys);});
            }

            if (allCustomFloatListRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomFloatListRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.floatLists, targetKeys);});
                }
            }

            if (allCustomFloatListRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomFloatListRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.floatLists, performerKeys);});
                }
            }
        }

        if (!anyCustomStringListRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomStringListRecordMatrix = StringUtil::toTagMatrix(anyCustomStringListRecord);

            std::vector<std::string> actorKeys = anyCustomStringListRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.stringLists, actorKeys);});
            }

            if (anyCustomStringListRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomStringListRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.stringLists, targetKeys);});
                }
            }

            if (anyCustomStringListRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomStringListRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.stringLists, performerKeys);});
                }
            }
        }

        if (!allCustomStringListRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomStringListRecordsMatrix = StringUtil::toTagMatrix(allCustomStringListRecords);

            std::vector<std::string> actorKeys = allCustomStringListRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.stringLists, actorKeys);});
            }

            if (allCustomStringListRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomStringListRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.stringLists, targetKeys);});
                }
            }

            if (allCustomStringListRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomStringListRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.stringLists, performerKeys);});
                }
            }
        }


        return findAction(id, [&conditions](Graph::Action action) {return checkConditions(conditions, action);});
    }

    std::vector<int> FindActionsSuperloadCSVv2(RE::StaticFunctionTag*, std::string id, std::string actorPositions, std::string targetPositions, std::string performerPositions, std::string matePositionsAny, std::string matePositionsAll, std::string participantPositionsAny, std::string participantPositionsAll, std::string types, std::string anyActionTag, std::string allActionTags, std::string actionTagWhitelist, std::string actionTagBlacklist, std::string anyCustomIntRecord, std::string allCustomIntRecords, std::string anyCustomFloatRecord, std::string allCustomFloatRecords, std::string anyCustomStringRecord, std::string allCustomStringRecords, std::string anyCustomIntListRecord, std::string allCustomIntListRecords, std::string anyCustomFloatListRecord, std::string allCustomFloatListRecords, std::string anyCustomStringListRecord, std::string allCustomStringListRecords) {
        std::vector<std::function<bool(Graph::Action)>> conditions;
        

        if (!actorPositions.empty()) {
            std::vector<int> actorPos = VectorUtil::stoiv(actorPositions);
            conditions.push_back([actorPos](Graph::Action action){return VectorUtil::contains(actorPos, action.actor);});
        }

        if (!targetPositions.empty()) {
            std::vector<int> targetPos = VectorUtil::stoiv(targetPositions);
            conditions.push_back([targetPos](Graph::Action action){return VectorUtil::contains(targetPos, action.target);});
        }

        if (!performerPositions.empty()) {
            std::vector<int> performerPos = VectorUtil::stoiv(performerPositions);
            conditions.push_back([performerPos](Graph::Action action){return VectorUtil::contains(performerPos, action.performer);});
        }

        if (!matePositionsAny.empty()) {
            std::vector<int> matePosAny = VectorUtil::stoiv(matePositionsAny);
            conditions.push_back([matePosAny](Graph::Action action){return VectorUtil::containsAny(matePosAny, {action.actor, action.target});});
        }

        if (!matePositionsAll.empty()) {
            std::vector<int> matePosAll = VectorUtil::stoiv(matePositionsAll);
            conditions.push_back([matePosAll](Graph::Action action){return VectorUtil::containsAll(matePosAll, {action.actor, action.target});});
        }

        if (!participantPositionsAny.empty()) {
            std::vector<int> participantPosAny = VectorUtil::stoiv(participantPositionsAny);
            conditions.push_back([participantPosAny](Graph::Action action){return VectorUtil::containsAny(participantPosAny, {action.actor, action.target, action.performer});});
        }

        if (!participantPositionsAll.empty()) {
            std::vector<int> participantPosAll = VectorUtil::stoiv(participantPositionsAll);
            conditions.push_back([participantPosAll](Graph::Action action){return VectorUtil::containsAll(participantPosAll, {action.actor, action.target, action.performer});});
        }

        if (!types.empty()) {
            std::vector<std::string> typeVector = StringUtil::toTagVector(types);
            conditions.push_back([typeVector](Graph::Action action){return VectorUtil::contains(typeVector, action.type);});
        }


        if (!anyActionTag.empty()) {
            std::vector<std::string> AnyActionTagVector = StringUtil::toTagVector(anyActionTag);
            conditions.push_back([AnyActionTagVector](Graph::Action action){return VectorUtil::containsAny(action.attributes->tags, AnyActionTagVector);});
        }

        if (!allActionTags.empty()) {
            std::vector<std::string> AllActionTagsVector = StringUtil::toTagVector(allActionTags);
            conditions.push_back([AllActionTagsVector](Graph::Action action){return VectorUtil::containsAll(action.attributes->tags, AllActionTagsVector);});
        }

        if (!actionTagWhitelist.empty()) {
            std::vector<std::string> actionTagWhitelistVector = StringUtil::toTagVector(actionTagWhitelist);
            conditions.push_back([actionTagWhitelistVector](Graph::Action action){return VectorUtil::containsAll(actionTagWhitelistVector, action.attributes->tags);});
        }

        if (!actionTagBlacklist.empty()) {
            std::vector<std::string> actionTagBlacklistVector = StringUtil::toTagVector(actionTagBlacklist);
            conditions.push_back([actionTagBlacklistVector](Graph::Action action){return !VectorUtil::containsAny(actionTagBlacklistVector, action.attributes->tags);});
        }


        if (!anyCustomIntRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomIntRecordMatrix = StringUtil::toTagMatrix(anyCustomIntRecord);

            std::vector<std::string> actorKeys = anyCustomIntRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.ints, actorKeys);});
            }

            if (anyCustomIntRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomIntRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.ints, targetKeys);});
                }
            }

            if (anyCustomIntRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomIntRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.ints, performerKeys);});
                }
            }
        }

        if (!allCustomIntRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomIntRecordsMatrix = StringUtil::toTagMatrix(allCustomIntRecords);

            std::vector<std::string> actorKeys = allCustomIntRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.ints, actorKeys);});
            }

            if (allCustomIntRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomIntRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.ints, targetKeys);});
                }
            }

            if (allCustomIntRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomIntRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.ints, performerKeys);});
                }
            }
        }

        if (!anyCustomFloatRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomFloatRecordMatrix = StringUtil::toTagMatrix(anyCustomFloatRecord);

            std::vector<std::string> actorKeys = anyCustomFloatRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.floats, actorKeys);});
            }

            if (anyCustomFloatRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomFloatRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.floats, targetKeys);});
                }
            }

            if (anyCustomFloatRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomFloatRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.floats, performerKeys);});
                }
            }
        }

        if (!allCustomFloatRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomFloatRecordsMatrix = StringUtil::toTagMatrix(allCustomFloatRecords);

            std::vector<std::string> actorKeys = allCustomFloatRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.floats, actorKeys);});
            }

            if (allCustomFloatRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomFloatRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.floats, targetKeys);});
                }
            }

            if (allCustomFloatRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomFloatRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.floats, performerKeys);});
                }
            }
        }

        if (!anyCustomStringRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomStringRecordMatrix = StringUtil::toTagMatrix(anyCustomStringRecord);

            std::vector<std::string> actorKeys = anyCustomStringRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.strings, actorKeys);});
            }

            if (anyCustomStringRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomStringRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.strings, targetKeys);});
                }
            }

            if (anyCustomStringRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomStringRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.strings, performerKeys);});
                }
            }
        }

        if (!allCustomStringRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomStringRecordsMatrix = StringUtil::toTagMatrix(allCustomStringRecords);

            std::vector<std::string> actorKeys = allCustomStringRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.strings, actorKeys);});
            }

            if (allCustomStringRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomStringRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.strings, targetKeys);});
                }
            }

            if (allCustomStringRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomStringRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.strings, performerKeys);});
                }
            }
        }

        if (!anyCustomIntListRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomIntListRecordMatrix = StringUtil::toTagMatrix(anyCustomIntListRecord);

            std::vector<std::string> actorKeys = anyCustomIntListRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.intLists, actorKeys);});
            }

            if (anyCustomIntListRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomIntListRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.intLists, targetKeys);});
                }
            }

            if (anyCustomIntListRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomIntListRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.intLists, performerKeys);});
                }
            }
        }

        if (!allCustomIntListRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomIntListRecordsMatrix = StringUtil::toTagMatrix(allCustomIntListRecords);

            std::vector<std::string> actorKeys = allCustomIntListRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.intLists, actorKeys);});
            }

            if (allCustomIntListRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomIntListRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.intLists, targetKeys);});
                }
            }

            if (allCustomIntListRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomIntListRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.intLists, performerKeys);});
                }
            }
        }

        if (!anyCustomFloatListRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomFloatListRecordMatrix = StringUtil::toTagMatrix(anyCustomFloatListRecord);

            std::vector<std::string> actorKeys = anyCustomFloatListRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.floatLists, actorKeys);});
            }

            if (anyCustomFloatListRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomFloatListRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.floatLists, targetKeys);});
                }
            }

            if (anyCustomFloatListRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomFloatListRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.floatLists, performerKeys);});
                }
            }
        }

        if (!allCustomFloatListRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomFloatListRecordsMatrix = StringUtil::toTagMatrix(allCustomFloatListRecords);

            std::vector<std::string> actorKeys = allCustomFloatListRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.floatLists, actorKeys);});
            }

            if (allCustomFloatListRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomFloatListRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.floatLists, targetKeys);});
                }
            }

            if (allCustomFloatListRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomFloatListRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.floatLists, performerKeys);});
                }
            }
        }

        if (!anyCustomStringListRecord.empty()) {
            std::vector<std::vector<std::string>> anyCustomStringListRecordMatrix = StringUtil::toTagMatrix(anyCustomStringListRecord);

            std::vector<std::string> actorKeys = anyCustomStringListRecordMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->actor.stringLists, actorKeys);});
            }

            if (anyCustomStringListRecordMatrix.size() > 1) {
                std::vector<std::string> targetKeys = anyCustomStringListRecordMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->target.stringLists, targetKeys);});
                }
            }

            if (anyCustomStringListRecordMatrix.size() > 2) {
                std::vector<std::string> performerKeys = anyCustomStringListRecordMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAnyKey(action.attributes->performer.stringLists, performerKeys);});
                }
            }
        }

        if (!allCustomStringListRecords.empty()) {
            std::vector<std::vector<std::string>> allCustomStringListRecordsMatrix = StringUtil::toTagMatrix(allCustomStringListRecords);

            std::vector<std::string> actorKeys = allCustomStringListRecordsMatrix[0];
            if (actorKeys.size() > 0) {
                conditions.push_back([&actorKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->actor.stringLists, actorKeys);});
            }

            if (allCustomStringListRecordsMatrix.size() > 1) {
                std::vector<std::string> targetKeys = allCustomStringListRecordsMatrix[1];
                if (targetKeys.size() > 0) {
                    conditions.push_back([&targetKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->target.stringLists, targetKeys);});
                }
            }

            if (allCustomStringListRecordsMatrix.size() > 2) {
                std::vector<std::string> performerKeys = allCustomStringListRecordsMatrix[2];
                if (performerKeys.size() > 0) {
                    conditions.push_back([&performerKeys](Graph::Action action) { return MapUtil::containsAllKeys(action.attributes->performer.stringLists, performerKeys);});
                }
            }
        }


        return findActions(id, [&conditions](Graph::Action action) {return checkConditions(conditions, action);});
    }
#pragma endregion
#pragma endregion

#pragma region action_properties
    std::vector<std::string> GetActionTypes(RE::StaticFunctionTag*, std::string id) {
        std::vector<std::string> ret;
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (auto& action : node->actions) {
                ret.push_back(action.type);
            }
        }
        return ret;
    }

    std::string GetActionType(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].type;
            }
        }
        return "";
    }

    std::vector<int> GetActionActors(RE::StaticFunctionTag*, std::string id) {
        std::vector<int> ret;
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (auto& action : node->actions) {
                ret.push_back(action.actor);
            }
        }
        return ret;
    }

    int GetActionActor(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].actor;
            }
        }
        return -1;
    }

    std::vector<int> GetActionTargets(RE::StaticFunctionTag*, std::string id) {
        std::vector<int> ret;
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (auto& action : node->actions) {
                ret.push_back(action.target);
            }
        }
        return ret;
    }

    int GetActionTarget(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].target;
            }
        }
        return -1;
    }

    std::vector<int> GetActionPerformers(RE::StaticFunctionTag*, std::string id) {
        std::vector<int> ret;
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (auto& action : node->actions) {
                ret.push_back(action.performer);
            }
        }
        return ret;
    }

    int GetActionPerformer(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].actor;
            }
        }
        return -1;
    }
#pragma endregion
    
#pragma region action_tags
    std::vector<std::string> GetActionTags(RE::StaticFunctionTag*, std::string id, int index) {
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->tags;
            }
        }
        return {};
    }

    std::vector<std::string> GetAllActionsTags(RE::StaticFunctionTag*, std::string id) {
        std::set<std::string> tags;
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                for (std::string tag : action.attributes->tags) {
                    tags.insert(tag);
                }
            }
        }
        return VectorUtil::toVector(tags);
    }

    bool HasActionTag(RE::StaticFunctionTag*, std::string id, int index, std::string tag) {
        StringUtil::toLower(&tag);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(node->actions[index].attributes->tags, tag);
            }
        }
        return false;
    }

    bool HasActionTagOnAny(RE::StaticFunctionTag*, std::string id, std::string tag) {
        StringUtil::toLower(&tag);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action: node->actions) {
                if (VectorUtil::contains(action.attributes->tags, tag)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool HasAnyActionTag(RE::StaticFunctionTag*, std::string id, int index, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(node->actions[index].attributes->tags, tags);
            }
        }
        return false;
    }

    bool HasAnyActionTagCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string tags) {
        return HasAnyActionTag(sft, id, index, StringUtil::toTagVector(tags));
    }

    bool HasAnyActionTagOnAny(RE::StaticFunctionTag*, std::string id, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (VectorUtil::containsAny(action.attributes->tags, tags)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool HasAnyActionTagOnAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string tags) {
        return HasAnyActionTagOnAny(sft, id, StringUtil::toTagVector(tags));
    }

    bool HasAllActionTags(RE::StaticFunctionTag*, std::string id, int index, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(node->actions[index].attributes->tags, tags);
            }
        }
        return false;
    }

    bool HasAllActionTagsCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string tags) {
        return HasAllActionTags(sft, id, index, StringUtil::toTagVector(tags));
    }

    bool HasAllActionTagsOnAny(RE::StaticFunctionTag*, std::string id, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (VectorUtil::containsAll(action.attributes->tags, tags)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool HasAllActionTagsOnAnyCSV(RE::StaticFunctionTag* sft, std::string id, std::string tags) {
        return HasAllActionTagsOnAny(sft, id, StringUtil::toTagVector(tags));
    }

    bool HasAllActionTagsOverAll(RE::StaticFunctionTag*, std::string id, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                std::erase_if(tags, [action](std::string tag) {return VectorUtil::contains(action.attributes->tags, tag);});
                if (tags.empty()) {
                    return true;
                }
            }
        }
        return false;
    }

    bool HasAllActionTagsOverAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string tags) {
        return HasAllActionTagsOverAll(sft, id, StringUtil::toTagVector(tags));
    }

    std::vector<std::string> GetActionTagOverlap(RE::StaticFunctionTag*, std::string id, int index, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(node->actions[index].attributes->tags, tags);
            }
        }

        return {};
    }

    std::vector<std::string> GetActionTagOverlapCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string tags) {
        return GetActionTagOverlap(sft, id, index, StringUtil::toTagVector(tags));
    }

    std::vector<std::string> GetActionTagOverlapOverAll(RE::StaticFunctionTag*, std::string id, std::vector<std::string> tags) {
        StringUtil::toLower(&tags);
        std::set<std::string> ret;

        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                for (auto& tag : action.attributes->tags) {
                    if (VectorUtil::contains(tags, tag)) {
                        ret.insert(tag);
                    }
                }
            }
        }

        return VectorUtil::toVector(ret);
    }

    std::vector<std::string> GetActionTagOverlapOverAllCSV(RE::StaticFunctionTag* sft, std::string id, std::string tags) {
        return GetActionTagOverlapOverAll(sft, id, StringUtil::toTagVector(tags));
    }
#pragma endregion

#pragma region custom_action_data
#pragma region custom_action_actor_data
#pragma region custom_action_actor_single_data
    bool HasCustomActionActorInt(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->actor.ints.contains(record);
            }
        }
        return false;
    }

    int GetCustomActionActorInt(RE::StaticFunctionTag*, std::string id, int index, std::string record, int fallback) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->actor.ints, record, fallback);
            }
        }
        return fallback;
    }

    bool IsCustomActionActorInt(RE::StaticFunctionTag*, std::string id, int index, std::string record, int value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::isValue(node->actions[index].attributes->actor.ints, record, value);
            }
        }
        return false;
    }

    bool HasCustomActionActorFloat(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->actor.floats.contains(record);
            }
        }
        return false;
    }

    float GetCustomActionActorFloat(RE::StaticFunctionTag*, std::string id, int index, std::string record, float fallback) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->actor.floats, record, fallback);
            }
        }
        return fallback;
    }

    bool IsCustomActionActorFloat(RE::StaticFunctionTag*, std::string id, int index, std::string record, float value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::isValue(node->actions[index].attributes->actor.floats, record, value);
            }
        }
        return false;
    }

    bool HasCustomActionActorString(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->actor.strings.contains(record);
            }
        }
        return false;
    }

    std::string GetCustomActionActorString(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::string fallback) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->actor.strings, record, fallback);
            }
        }
        return fallback;
    }

    bool IsCustomActionActorString(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::string value) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&value);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::isValue(node->actions[index].attributes->actor.strings, record, value);
            }
        }
        return false;
    }
#pragma endregion

#pragma region custom_action_actor_int_lists
    bool HasCustomActionActorIntList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->actor.intLists.contains(record);
            }
        }
        return false;
    }

    std::vector<int> GetCustomActionActorIntList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->actor.intLists, record, {});
            }
        }
        return {};
    }

    bool CustomActionActorIntListContains(RE::StaticFunctionTag*, std::string id, int index, std::string record, int value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(MapUtil::getOrFallback(node->actions[index].attributes->actor.intLists, record, {}), value);
            }
        }
        return false;
    }

    bool CustomActionActorIntListContainsAny(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<int> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(MapUtil::getOrFallback(node->actions[index].attributes->actor.intLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionActorIntListContainsAnyCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionActorIntListContainsAny(sft, id, index, record, VectorUtil::stoiv(values));
    }

    bool CustomActionActorIntListContainsAll(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<int> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(MapUtil::getOrFallback(node->actions[index].attributes->actor.intLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionActorIntListContainsAllCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionActorIntListContainsAll(sft, id, index, record, VectorUtil::stoiv(values));
    }

    std::vector<int> GetCustomActionActorIntListOverlap(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<int> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(MapUtil::getOrFallback(node->actions[index].attributes->actor.intLists, record, {}), values);
            }
        }
        return {};
    }

    std::vector<int> GetCustomActionActorIntListOverlapCSV(RE::StaticFunctionTag*sft , std::string id, int index, std::string record, std::string values) {
        return GetCustomActionActorIntListOverlap(sft, id, index, record, VectorUtil::stoiv(values));
    }
#pragma endregion

#pragma region custom_action_actor_float_lists
    bool HasCustomActionActorFloatList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->actor.floatLists.contains(record);
            }
        }
        return false;
    }

    std::vector<float> GetCustomActionActorFloatList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->actor.floatLists, record, {});
            }
        }
        return {};
    }

    bool CustomActionActorFloatListContains(RE::StaticFunctionTag*, std::string id, int index, std::string record, float value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(MapUtil::getOrFallback(node->actions[index].attributes->actor.floatLists, record, {}), value);
            }
        }
        return false;
    }

    bool CustomActionActorFloatListContainsAny(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<float> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(MapUtil::getOrFallback(node->actions[index].attributes->actor.floatLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionActorFloatListContainsAnyCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionActorFloatListContainsAny(sft, id, index, record, VectorUtil::stofv(values));
    }

    bool CustomActionActorFloatListContainsAll(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<float> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(MapUtil::getOrFallback(node->actions[index].attributes->actor.floatLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionActorFloatListContainsAllCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionActorFloatListContainsAll(sft, id, index, record, VectorUtil::stofv(values));
    }

    std::vector<float> GetCustomActionActorFloatListOverlap(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<float> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(MapUtil::getOrFallback(node->actions[index].attributes->actor.floatLists, record, {}), values);
            }
        }
        return {};
    }

    std::vector<float> GetCustomActionActorFloatListOverlapCSV(RE::StaticFunctionTag*sft , std::string id, int index, std::string record, std::string values) {
        return GetCustomActionActorFloatListOverlap(sft, id, index, record, VectorUtil::stofv(values));
    }
#pragma endregion

#pragma region custom_action_actor_string_lists
    bool HasCustomActionActorStringList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->actor.stringLists.contains(record);
            }
        }
        return false;
    }

    std::vector<std::string> GetCustomActionActorStringList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->actor.stringLists, record, {});
            }
        }
        return {};
    }

    bool CustomActionActorStringListContains(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::string value) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&value);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(MapUtil::getOrFallback(node->actions[index].attributes->actor.stringLists, record, {}), value);
            }
        }
        return false;
    }

    bool CustomActionActorStringListContainsAny(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<std::string> values) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&values);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(MapUtil::getOrFallback(node->actions[index].attributes->actor.stringLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionActorStringListContainsAnyCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionActorStringListContainsAny(sft, id, index, record, StringUtil::toTagVector(values));
    }

    bool CustomActionActorStringListContainsAll(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<std::string> values) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&values);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(MapUtil::getOrFallback(node->actions[index].attributes->actor.stringLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionActorStringListContainsAllCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionActorStringListContainsAll(sft, id, index, record, StringUtil::toTagVector(values));
    }

    std::vector<std::string> GetCustomActionActorStringListOverlap(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<std::string> values) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&values);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(MapUtil::getOrFallback(node->actions[index].attributes->actor.stringLists, record, {}), values);
            }
        }
        return {};
    }

    std::vector<std::string> GetCustomActionActorStringListOverlapCSV(RE::StaticFunctionTag*sft , std::string id, int index, std::string record, std::string values) {
        return GetCustomActionActorStringListOverlap(sft, id, index, record, StringUtil::toTagVector(values));
    }
#pragma endregion
#pragma endregion

#pragma region custom_action_target_data
#pragma region custom_action_target_single_data
    bool HasCustomActionTargetInt(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->target.ints.contains(record);
            }
        }
        return false;
    }

    int GetCustomActionTargetInt(RE::StaticFunctionTag*, std::string id, int index, std::string record, int fallback) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->target.ints, record, fallback);
            }
        }
        return fallback;
    }

    bool IsCustomActionTargetInt(RE::StaticFunctionTag*, std::string id, int index, std::string record, int value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::isValue(node->actions[index].attributes->target.ints, record, value);
            }
        }
        return false;
    }

    bool HasCustomActionTargetFloat(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->target.floats.contains(record);
            }
        }
        return false;
    }

    float GetCustomActionTargetFloat(RE::StaticFunctionTag*, std::string id, int index, std::string record, float fallback) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->target.floats, record, fallback);
            }
        }
        return fallback;
    }

    bool IsCustomActionTargetFloat(RE::StaticFunctionTag*, std::string id, int index, std::string record, float value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::isValue(node->actions[index].attributes->target.floats, record, value);
            }
        }
        return false;
    }

    bool HasCustomActionTargetString(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->target.strings.contains(record);
            }
        }
        return false;
    }

    std::string GetCustomActionTargetString(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::string fallback) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->target.strings, record, fallback);
            }
        }
        return fallback;
    }

    bool IsCustomActionTargetString(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::string value) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&value);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::isValue(node->actions[index].attributes->target.strings, record, value);
            }
        }
        return false;
    }
#pragma endregion

#pragma region custom_action_target_int_lists
    bool HasCustomActionTargetIntList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->target.intLists.contains(record);
            }
        }
        return false;
    }

    std::vector<int> GetCustomActionTargetIntList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->target.intLists, record, {});
            }
        }
        return {};
    }

    bool CustomActionTargetIntListContains(RE::StaticFunctionTag*, std::string id, int index, std::string record, int value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(MapUtil::getOrFallback(node->actions[index].attributes->target.intLists, record, {}), value);
            }
        }
        return false;
    }

    bool CustomActionTargetIntListContainsAny(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<int> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(MapUtil::getOrFallback(node->actions[index].attributes->target.intLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionTargetIntListContainsAnyCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionTargetIntListContainsAny(sft, id, index, record, VectorUtil::stoiv(values));
    }

    bool CustomActionTargetIntListContainsAll(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<int> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(MapUtil::getOrFallback(node->actions[index].attributes->target.intLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionTargetIntListContainsAllCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionTargetIntListContainsAll(sft, id, index, record, VectorUtil::stoiv(values));
    }

    std::vector<int> GetCustomActionTargetIntListOverlap(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<int> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(MapUtil::getOrFallback(node->actions[index].attributes->target.intLists, record, {}), values);
            }
        }
        return {};
    }

    std::vector<int> GetCustomActionTargetIntListOverlapCSV(RE::StaticFunctionTag*sft , std::string id, int index, std::string record, std::string values) {
        return GetCustomActionTargetIntListOverlap(sft, id, index, record, VectorUtil::stoiv(values));
    }
#pragma endregion

#pragma region custom_action_target_float_lists
    bool HasCustomActionTargetFloatList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->target.floatLists.contains(record);
            }
        }
        return false;
    }

    std::vector<float> GetCustomActionTargetFloatList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->target.floatLists, record, {});
            }
        }
        return {};
    }

    bool CustomActionTargetFloatListContains(RE::StaticFunctionTag*, std::string id, int index, std::string record, float value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(MapUtil::getOrFallback(node->actions[index].attributes->target.floatLists, record, {}), value);
            }
        }
        return false;
    }

    bool CustomActionTargetFloatListContainsAny(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<float> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(MapUtil::getOrFallback(node->actions[index].attributes->target.floatLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionTargetFloatListContainsAnyCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionTargetFloatListContainsAny(sft, id, index, record, VectorUtil::stofv(values));
    }

    bool CustomActionTargetFloatListContainsAll(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<float> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(MapUtil::getOrFallback(node->actions[index].attributes->target.floatLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionTargetFloatListContainsAllCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionTargetFloatListContainsAll(sft, id, index, record, VectorUtil::stofv(values));
    }

    std::vector<float> GetCustomActionTargetFloatListOverlap(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<float> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(MapUtil::getOrFallback(node->actions[index].attributes->target.floatLists, record, {}), values);
            }
        }
        return {};
    }

    std::vector<float> GetCustomActionTargetFloatListOverlapCSV(RE::StaticFunctionTag*sft , std::string id, int index, std::string record, std::string values) {
        return GetCustomActionTargetFloatListOverlap(sft, id, index, record, VectorUtil::stofv(values));
    }
#pragma endregion

#pragma region custom_action_target_string_lists
    bool HasCustomActionTargetStringList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->target.stringLists.contains(record);
            }
        }
        return false;
    }

    std::vector<std::string> GetCustomActionTargetStringList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->target.stringLists, record, {});
            }
        }
        return {};
    }

    bool CustomActionTargetStringListContains(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::string value) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&value);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(MapUtil::getOrFallback(node->actions[index].attributes->target.stringLists, record, {}), value);
            }
        }
        return false;
    }

    bool CustomActionTargetStringListContainsAny(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<std::string> values) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&values);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(MapUtil::getOrFallback(node->actions[index].attributes->target.stringLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionTargetStringListContainsAnyCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionTargetStringListContainsAny(sft, id, index, record, StringUtil::toTagVector(values));
    }

    bool CustomActionTargetStringListContainsAll(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<std::string> values) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&values);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(MapUtil::getOrFallback(node->actions[index].attributes->target.stringLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionTargetStringListContainsAllCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionTargetStringListContainsAll(sft, id, index, record, StringUtil::toTagVector(values));
    }

    std::vector<std::string> GetCustomActionTargetStringListOverlap(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<std::string> values) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&values);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(MapUtil::getOrFallback(node->actions[index].attributes->target.stringLists, record, {}), values);
            }
        }
        return {};
    }

    std::vector<std::string> GetCustomActionTargetStringListOverlapCSV(RE::StaticFunctionTag*sft , std::string id, int index, std::string record, std::string values) {
        return GetCustomActionTargetStringListOverlap(sft, id, index, record, StringUtil::toTagVector(values));
    }
#pragma endregion
#pragma endregion

#pragma region custom_action_performer_data
#pragma region custom_action_performer_single_data
    bool HasCustomActionPerformerInt(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->performer.ints.contains(record);
            }
        }
        return false;
    }

    int GetCustomActionPerformerInt(RE::StaticFunctionTag*, std::string id, int index, std::string record, int fallback) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->performer.ints, record, fallback);
            }
        }
        return fallback;
    }

    bool IsCustomActionPerformerInt(RE::StaticFunctionTag*, std::string id, int index, std::string record, int value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::isValue(node->actions[index].attributes->performer.ints, record, value);
            }
        }
        return false;
    }

    bool HasCustomActionPerformerFloat(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->performer.floats.contains(record);
            }
        }
        return false;
    }

    float GetCustomActionPerformerFloat(RE::StaticFunctionTag*, std::string id, int index, std::string record, float fallback) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->performer.floats, record, fallback);
            }
        }
        return fallback;
    }

    bool IsCustomActionPerformerFloat(RE::StaticFunctionTag*, std::string id, int index, std::string record, float value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::isValue(node->actions[index].attributes->performer.floats, record, value);
            }
        }
        return false;
    }

    bool HasCustomActionPerformerString(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->performer.strings.contains(record);
            }
        }
        return false;
    }

    std::string GetCustomActionPerformerString(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::string fallback) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->performer.strings, record, fallback);
            }
        }
        return fallback;
    }

    bool IsCustomActionPerformerString(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::string value) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&value);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::isValue(node->actions[index].attributes->performer.strings, record, value);
            }
        }
        return false;
    }
#pragma endregion

#pragma region custom_action_performer_int_lists
    bool HasCustomActionPerformerIntList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->performer.intLists.contains(record);
            }
        }
        return false;
    }

    std::vector<int> GetCustomActionPerformerIntList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->performer.intLists, record, {});
            }
        }
        return {};
    }

    bool CustomActionPerformerIntListContains(RE::StaticFunctionTag*, std::string id, int index, std::string record, int value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(MapUtil::getOrFallback(node->actions[index].attributes->performer.intLists, record, {}), value);
            }
        }
        return false;
    }

    bool CustomActionPerformerIntListContainsAny(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<int> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(MapUtil::getOrFallback(node->actions[index].attributes->performer.intLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionPerformerIntListContainsAnyCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionPerformerIntListContainsAny(sft, id, index, record, VectorUtil::stoiv(values));
    }

    bool CustomActionPerformerIntListContainsAll(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<int> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(MapUtil::getOrFallback(node->actions[index].attributes->performer.intLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionPerformerIntListContainsAllCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionPerformerIntListContainsAll(sft, id, index, record, VectorUtil::stoiv(values));
    }

    std::vector<int> GetCustomActionPerformerIntListOverlap(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<int> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(MapUtil::getOrFallback(node->actions[index].attributes->performer.intLists, record, {}), values);
            }
        }
        return {};
    }

    std::vector<int> GetCustomActionPerformerIntListOverlapCSV(RE::StaticFunctionTag*sft , std::string id, int index, std::string record, std::string values) {
        return GetCustomActionPerformerIntListOverlap(sft, id, index, record, VectorUtil::stoiv(values));
    }
#pragma endregion

#pragma region custom_action_performer_float_lists
    bool HasCustomActionPerformerFloatList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->performer.floatLists.contains(record);
            }
        }
        return false;
    }

    std::vector<float> GetCustomActionPerformerFloatList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->performer.floatLists, record, {});
            }
        }
        return {};
    }

    bool CustomActionPerformerFloatListContains(RE::StaticFunctionTag*, std::string id, int index, std::string record, float value) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(MapUtil::getOrFallback(node->actions[index].attributes->performer.floatLists, record, {}), value);
            }
        }
        return false;
    }

    bool CustomActionPerformerFloatListContainsAny(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<float> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(MapUtil::getOrFallback(node->actions[index].attributes->performer.floatLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionPerformerFloatListContainsAnyCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionPerformerFloatListContainsAny(sft, id, index, record, VectorUtil::stofv(values));
    }

    bool CustomActionPerformerFloatListContainsAll(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<float> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(MapUtil::getOrFallback(node->actions[index].attributes->performer.floatLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionPerformerFloatListContainsAllCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionPerformerFloatListContainsAll(sft, id, index, record, VectorUtil::stofv(values));
    }

    std::vector<float> GetCustomActionPerformerFloatListOverlap(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<float> values) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(MapUtil::getOrFallback(node->actions[index].attributes->performer.floatLists, record, {}), values);
            }
        }
        return {};
    }

    std::vector<float> GetCustomActionPerformerFloatListOverlapCSV(RE::StaticFunctionTag*sft , std::string id, int index, std::string record, std::string values) {
        return GetCustomActionPerformerFloatListOverlap(sft, id, index, record, VectorUtil::stofv(values));
    }
#pragma endregion

#pragma region custom_action_performer_string_lists
    bool HasCustomActionPerformerStringList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return node->actions[index].attributes->performer.stringLists.contains(record);
            }
        }
        return false;
    }

    std::vector<std::string> GetCustomActionPerformerStringList(RE::StaticFunctionTag*, std::string id, int index, std::string record) {
        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return MapUtil::getOrFallback(node->actions[index].attributes->performer.stringLists, record, {});
            }
        }
        return {};
    }

    bool CustomActionPerformerStringListContains(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::string value) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&value);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::contains(MapUtil::getOrFallback(node->actions[index].attributes->performer.stringLists, record, {}), value);
            }
        }
        return false;
    }

    bool CustomActionPerformerStringListContainsAny(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<std::string> values) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&values);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAny(MapUtil::getOrFallback(node->actions[index].attributes->performer.stringLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionPerformerStringListContainsAnyCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionPerformerStringListContainsAny(sft, id, index, record, StringUtil::toTagVector(values));
    }

    bool CustomActionPerformerStringListContainsAll(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<std::string> values) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&values);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::containsAll(MapUtil::getOrFallback(node->actions[index].attributes->performer.stringLists, record, {}), values);
            }
        }
        return false;
    }

    bool CustomActionPerformerStringListContainsAllCSV(RE::StaticFunctionTag* sft, std::string id, int index, std::string record, std::string values) {
        return CustomActionPerformerStringListContainsAll(sft, id, index, record, StringUtil::toTagVector(values));
    }

    std::vector<std::string> GetCustomActionPerformerStringListOverlap(RE::StaticFunctionTag*, std::string id, int index, std::string record, std::vector<std::string> values) {
        StringUtil::toLower(&record);
        StringUtil::toLower(&values);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            if (index < node->actions.size()) {
                return VectorUtil::getOverlap(MapUtil::getOrFallback(node->actions[index].attributes->performer.stringLists, record, {}), values);
            }
        }
        return {};
    }

    std::vector<std::string> GetCustomActionPerformerStringListOverlapCSV(RE::StaticFunctionTag*sft , std::string id, int index, std::string record, std::string values) {
        return GetCustomActionPerformerStringListOverlap(sft, id, index, record, StringUtil::toTagVector(values));
    }
#pragma endregion
#pragma endregion

#pragma region custom_scene_actor_data
    int GetCustomSceneActorIntMin(RE::StaticFunctionTag*, std::string id, int position, std::string record, int fallback) {
        int min = INT_MAX;

        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (action.actor == position) {
                    auto it = action.attributes->actor.ints.find(record);
                    if (it != action.attributes->actor.ints.end()) {
                        if (min > it->second) {
                            min = it->second;
                        }
                    }
                }
                if (action.target == position) {
                    auto it = action.attributes->target.ints.find(record);
                    if (it != action.attributes->target.ints.end()) {
                        if (min > it->second) {
                            min = it->second;
                        }
                    }
                }
                if (action.performer == position) {
                    auto it = action.attributes->performer.ints.find(record);
                    if (it != action.attributes->performer.ints.end()) {
                        if (min > it->second) {
                            min = it->second;
                        }
                    }
                }
            }
        }

        return min == INT_MAX ? fallback : min;
    }

    int GetCustomSceneActorIntMax(RE::StaticFunctionTag*, std::string id, int position, std::string record, int fallback) {
        int max = INT_MIN;

        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (action.actor == position) {
                    auto it = action.attributes->actor.ints.find(record);
                    if (it != action.attributes->actor.ints.end()) {
                        if (max < it->second) {
                            max = it->second;
                        }
                    }
                }
                if (action.target == position) {
                    auto it = action.attributes->target.ints.find(record);
                    if (it != action.attributes->target.ints.end()) {
                        if (max < it->second) {
                            max = it->second;
                        }
                    }
                }
                if (action.performer == position) {
                    auto it = action.attributes->performer.ints.find(record);
                    if (it != action.attributes->performer.ints.end()) {
                        if (max < it->second) {
                            max = it->second;
                        }
                    }
                }
            }
        }

        return max == INT_MIN ? fallback : max;
    }

    int GetCustomSceneActorIntSum(RE::StaticFunctionTag*, std::string id, int position, std::string record, int startValue) {
        int sum = startValue;

        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (action.actor == position) {
                    auto it = action.attributes->actor.ints.find(record);
                    if (it != action.attributes->actor.ints.end()) {
                        sum += it->second;
                    }
                }
                if (action.target == position) {
                    auto it = action.attributes->target.ints.find(record);
                    if (it != action.attributes->target.ints.end()) {
                        sum += it->second;
                    }
                }
                if (action.performer == position) {
                    auto it = action.attributes->performer.ints.find(record);
                    if (it != action.attributes->performer.ints.end()) {
                        sum += it->second;
                    }
                }
            }
        }

        return sum;
    }

    int GetCustomSceneActorIntProduct(RE::StaticFunctionTag*, std::string id, int position, std::string record, int startValue) {
        int product = startValue;

        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (action.actor == position) {
                    auto it = action.attributes->actor.ints.find(record);
                    if (it != action.attributes->actor.ints.end()) {
                        product *= it->second;
                    }
                }
                if (action.target == position) {
                    auto it = action.attributes->target.ints.find(record);
                    if (it != action.attributes->target.ints.end()) {
                        product *= it->second;
                    }
                }
                if (action.performer == position) {
                    auto it = action.attributes->performer.ints.find(record);
                    if (it != action.attributes->performer.ints.end()) {
                        product *= it->second;
                    }
                }
            }
        }

        return product;
    }

    
    float GetCustomSceneActorFloatMin(RE::StaticFunctionTag*, std::string id, int position, std::string record, float fallback) {
        float min = std::numeric_limits<float>::quiet_NaN();

        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (action.actor == position) {
                    auto it = action.attributes->actor.floats.find(record);
                    if (it != action.attributes->actor.floats.end()) {
                        if (isnan(min) || min > it->second) {
                            min = it->second;
                        }
                    }
                }
                if (action.target == position) {
                    auto it = action.attributes->target.floats.find(record);
                    if (it != action.attributes->target.floats.end()) {
                        if (isnan(min) || min > it->second) {
                            min = it->second;
                        }
                    }
                }
                if (action.performer == position) {
                    auto it = action.attributes->performer.floats.find(record);
                    if (it != action.attributes->performer.floats.end()) {
                        if (isnan(min) || min > it->second) {
                            min = it->second;
                        }
                    }
                }
            }
        }

        return isnan(min) ? fallback : min;
    }

    float GetCustomSceneActorFloatMax(RE::StaticFunctionTag*, std::string id, int position, std::string record, float fallback) {
        float max = std::numeric_limits<float>::quiet_NaN();

        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (action.actor == position) {
                    auto it = action.attributes->actor.floats.find(record);
                    if (it != action.attributes->actor.floats.end()) {
                        if (isnan(max) || max < it->second) {
                            max = it->second;
                        }
                    }
                }
                if (action.target == position) {
                    auto it = action.attributes->target.floats.find(record);
                    if (it != action.attributes->target.floats.end()) {
                        if (isnan(max) || max < it->second) {
                            max = it->second;
                        }
                    }
                }
                if (action.performer == position) {
                    auto it = action.attributes->performer.floats.find(record);
                    if (it != action.attributes->performer.floats.end()) {
                        if (isnan(max) || max < it->second) {
                            max = it->second;
                        }
                    }
                }
            }
        }

        return isnan(max) ? fallback : max;
    }

    float GetCustomSceneActorFloatSum(RE::StaticFunctionTag*, std::string id, int position, std::string record, float startValue) {
        float sum = startValue;

        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (action.actor == position) {
                    auto it = action.attributes->actor.floats.find(record);
                    if (it != action.attributes->actor.floats.end()) {
                        sum += it->second;
                    }
                }
                if (action.target == position) {
                    auto it = action.attributes->target.floats.find(record);
                    if (it != action.attributes->target.floats.end()) {
                        sum += it->second;
                    }
                }
                if (action.performer == position) {
                    auto it = action.attributes->performer.floats.find(record);
                    if (it != action.attributes->performer.floats.end()) {
                        sum += it->second;
                    }
                }
            }
        }

        return sum;
    }

    float GetCustomSceneActorFloatProduct(RE::StaticFunctionTag*, std::string id, int position, std::string record, float startValue) {
        float product = startValue;

        StringUtil::toLower(&record);
        if (auto node = Graph::GraphTable::getNodeById(id)) {
            for (Graph::Action action : node->actions) {
                if (action.actor == position) {
                    auto it = action.attributes->actor.floats.find(record);
                    if (it != action.attributes->actor.floats.end()) {
                        product *= it->second;
                    }
                }
                if (action.target == position) {
                    auto it = action.attributes->target.floats.find(record);
                    if (it != action.attributes->target.floats.end()) {
                        product *= it->second;
                    }
                }
                if (action.performer == position) {
                    auto it = action.attributes->performer.floats.find(record);
                    if (it != action.attributes->performer.floats.end()) {
                        product *= it->second;
                    }
                }
            }
        }

        return product;
    }
#pragma endregion
#pragma endregion
#pragma endregion


	bool Bind(VM* a_vm) {
        const auto obj = "OMetadata"sv;

        BIND(IsTransition);
        BIND(GetDefaultSpeed);
        BIND(GetMaxSpeed);
        BIND(GetActorCount);
        BIND(GetAnimationId);
        BIND(GetAutoTransitionForActor);

        BIND(GetSceneTags);
        BIND(HasSceneTag);
        BIND(HasAnySceneTag);
        BIND(HasAnySceneTagCSV);
        BIND(HasAllSceneTags);
        BIND(HasAllSceneTagsCSV);
        BIND(GetSceneTagOverlap);
        BIND(GetSceneTagOverlapCSV);

        BIND(GetActorTags);
        BIND(HasActorTag);
        BIND(HasAnyActorTag);
        BIND(HasAnyActorTagCSV);
        BIND(HasAllActorTags);
        BIND(HasAllActorTagsCSV);
        BIND(GetActorTagOverlap);
        BIND(GetActorTagOverlapCSV);

        BIND(HasActions);
        BIND(FindAction);
        BIND(FindAnyAction);
        BIND(FindAnyActionCSV);
        BIND(FindActions);
        BIND(FindAllActions);
        BIND(FindAllActionsCSV);

        BIND(FindActionForActor);
        BIND(FindAnyActionForActor);
        BIND(FindAnyActionForActorCSV);
        BIND(FindActionsForActor);
        BIND(FindAllActionsForActor);
        BIND(FindAllActionsForActorCSV);
        BIND(FindActionForActors);
        BIND(FindActionForActorsCSV);
        BIND(FindAnyActionForActors);
        BIND(FindAnyActionForActorsCSV);
        BIND(FindActionsForActors);
        BIND(FindActionsForActorsCSV);
        BIND(FindAllActionsForActors);
        BIND(FindAllActionsForActorsCSV);

        BIND(FindActionForTarget);
        BIND(FindAnyActionForTarget);
        BIND(FindAnyActionForTargetCSV);
        BIND(FindActionsForTarget);
        BIND(FindAllActionsForTarget);
        BIND(FindAllActionsForTargetCSV);
        BIND(FindActionForTargets);
        BIND(FindActionForTargetsCSV);
        BIND(FindAnyActionForTargets);
        BIND(FindAnyActionForTargetsCSV);
        BIND(FindActionsForTargets);
        BIND(FindActionsForTargetsCSV);
        BIND(FindAllActionsForTargets);
        BIND(FindAllActionsForTargetsCSV);

        BIND(FindActionForPerformer);
        BIND(FindAnyActionForPerformer);
        BIND(FindAnyActionForPerformerCSV);
        BIND(FindActionsForPerformer);
        BIND(FindAllActionsForPerformer);
        BIND(FindAllActionsForPerformerCSV);
        BIND(FindActionForPerformers);
        BIND(FindActionForPerformersCSV);
        BIND(FindAnyActionForPerformers);
        BIND(FindAnyActionForPerformersCSV);
        BIND(FindActionsForPerformers);
        BIND(FindActionsForPerformersCSV);
        BIND(FindAllActionsForPerformers);
        BIND(FindAllActionsForPerformersCSV);

        BIND(FindActionForActorAndTarget);
        BIND(FindAnyActionForActorAndTarget);
        BIND(FindAnyActionForActorAndTargetCSV);
        BIND(FindActionsForActorAndTarget);
        BIND(FindAllActionsForActorAndTarget);
        BIND(FindAllActionsForActorAndTargetCSV);
        BIND(FindActionForActorsAndTargets);
        BIND(FindActionForActorsAndTargetsCSV);
        BIND(FindAnyActionForActorsAndTargets);
        BIND(FindAnyActionForActorsAndTargetsCSV);
        BIND(FindActionsForActorsAndTargets);
        BIND(FindActionsForActorsAndTargetsCSV);
        BIND(FindAllActionsForActorsAndTargets);
        BIND(FindAllActionsForActorsAndTargetsCSV);

        BIND(FindActionForMate);
        BIND(FindAnyActionForMate);
        BIND(FindAnyActionForMateCSV);
        BIND(FindActionsForMate);
        BIND(FindAllActionsForMate);
        BIND(FindAllActionsForMateCSV);
        BIND(FindActionForMatesAny);
        BIND(FindActionForMatesAnyCSV);
        BIND(FindAnyActionForMatesAny);
        BIND(FindAnyActionForMatesAnyCSV);
        BIND(FindActionsForMatesAny);
        BIND(FindActionsForMatesAnyCSV);
        BIND(FindAllActionsForMatesAny);
        BIND(FindAllActionsForMatesAnyCSV);
        BIND(FindActionForMatesAll);
        BIND(FindActionForMatesAllCSV);
        BIND(FindAnyActionForMatesAll);
        BIND(FindAnyActionForMatesAllCSV);
        BIND(FindActionsForMatesAll);
        BIND(FindActionsForMatesAllCSV);
        BIND(FindAllActionsForMatesAll);
        BIND(FindAllActionsForMatesAllCSV);

        BIND(FindActionForParticipant);
        BIND(FindAnyActionForParticipant);
        BIND(FindAnyActionForParticipantCSV);
        BIND(FindActionsForParticipant);
        BIND(FindAllActionsForParticipant);
        BIND(FindAllActionsForParticipantCSV);
        BIND(FindActionForParticipantsAny);
        BIND(FindActionForParticipantsAnyCSV);
        BIND(FindAnyActionForParticipantsAny);
        BIND(FindAnyActionForParticipantsAnyCSV);
        BIND(FindActionsForParticipantsAny);
        BIND(FindActionsForParticipantsAnyCSV);
        BIND(FindAllActionsForParticipantsAny);
        BIND(FindAllActionsForParticipantsAnyCSV);
        BIND(FindActionForParticipantsAll);
        BIND(FindActionForParticipantsAllCSV);
        BIND(FindAnyActionForParticipantsAll);
        BIND(FindAnyActionForParticipantsAllCSV);
        BIND(FindActionsForParticipantsAll);
        BIND(FindActionsForParticipantsAllCSV);
        BIND(FindAllActionsForParticipantsAll);
        BIND(FindAllActionsForParticipantsAllCSV);

        BIND(FindActionSuperloadCSVv2);
        BIND(FindActionsSuperloadCSVv2);

        BIND(GetActionTypes);
        BIND(GetActionType);
        BIND(GetActionActors);
        BIND(GetActionActor);
        BIND(GetActionTargets);
        BIND(GetActionTarget);
        BIND(GetActionPerformers);
        BIND(GetActionPerformer);

        BIND(GetActionTags);
        BIND(GetAllActionsTags);
        BIND(HasActionTag);
        BIND(HasActionTagOnAny);
        BIND(HasAnyActionTag);
        BIND(HasAnyActionTagCSV);
        BIND(HasAnyActionTagOnAny);
        BIND(HasAnyActionTagOnAnyCSV);
        BIND(HasAllActionTags);
        BIND(HasAllActionTagsCSV);
        BIND(HasAllActionTagsOnAny);
        BIND(HasAllActionTagsOnAnyCSV);
        BIND(HasAllActionTagsOverAll);
        BIND(HasAllActionTagsOverAllCSV);
        BIND(GetActionTagOverlap);
        BIND(GetActionTagOverlapCSV);
        BIND(GetActionTagOverlapOverAll);
        BIND(GetActionTagOverlapOverAllCSV);

        BIND(HasCustomActionActorInt);
        BIND(GetCustomActionActorInt);
        BIND(IsCustomActionActorInt);
        BIND(HasCustomActionActorFloat);
        BIND(GetCustomActionActorFloat);
        BIND(IsCustomActionActorFloat);
        BIND(HasCustomActionActorString);
        BIND(GetCustomActionActorString);
        BIND(IsCustomActionActorString);

        BIND(HasCustomActionActorIntList);
        BIND(GetCustomActionActorIntList);
        BIND(CustomActionActorIntListContains);
        BIND(CustomActionActorIntListContainsAny);
        BIND(CustomActionActorIntListContainsAnyCSV);
        BIND(CustomActionActorIntListContainsAll);
        BIND(CustomActionActorIntListContainsAllCSV);
        BIND(GetCustomActionActorIntListOverlap);
        BIND(GetCustomActionActorIntListOverlapCSV);

        BIND(HasCustomActionActorFloatList);
        BIND(GetCustomActionActorFloatList);
        BIND(CustomActionActorFloatListContains);
        BIND(CustomActionActorFloatListContainsAny);
        BIND(CustomActionActorFloatListContainsAnyCSV);
        BIND(CustomActionActorFloatListContainsAll);
        BIND(CustomActionActorFloatListContainsAllCSV);
        BIND(GetCustomActionActorFloatListOverlap);
        BIND(GetCustomActionActorFloatListOverlapCSV);

        BIND(HasCustomActionActorStringList);
        BIND(GetCustomActionActorStringList);
        BIND(CustomActionActorStringListContains);
        BIND(CustomActionActorStringListContainsAny);
        BIND(CustomActionActorStringListContainsAnyCSV);
        BIND(CustomActionActorStringListContainsAll);
        BIND(CustomActionActorStringListContainsAllCSV);
        BIND(GetCustomActionActorStringListOverlap);
        BIND(GetCustomActionActorStringListOverlapCSV);

        BIND(HasCustomActionTargetInt);
        BIND(GetCustomActionTargetInt);
        BIND(IsCustomActionTargetInt);
        BIND(HasCustomActionTargetFloat);
        BIND(GetCustomActionTargetFloat);
        BIND(IsCustomActionTargetFloat);
        BIND(HasCustomActionTargetString);
        BIND(GetCustomActionTargetString);
        BIND(IsCustomActionTargetString);

        BIND(HasCustomActionTargetIntList);
        BIND(GetCustomActionTargetIntList);
        BIND(CustomActionTargetIntListContains);
        BIND(CustomActionTargetIntListContainsAny);
        BIND(CustomActionTargetIntListContainsAnyCSV);
        BIND(CustomActionTargetIntListContainsAll);
        BIND(CustomActionTargetIntListContainsAllCSV);
        BIND(GetCustomActionTargetIntListOverlap);
        BIND(GetCustomActionTargetIntListOverlapCSV);

        BIND(HasCustomActionTargetFloatList);
        BIND(GetCustomActionTargetFloatList);
        BIND(CustomActionTargetFloatListContains);
        BIND(CustomActionTargetFloatListContainsAny);
        BIND(CustomActionTargetFloatListContainsAnyCSV);
        BIND(CustomActionTargetFloatListContainsAll);
        BIND(CustomActionTargetFloatListContainsAllCSV);
        BIND(GetCustomActionTargetFloatListOverlap);
        BIND(GetCustomActionTargetFloatListOverlapCSV);

        BIND(HasCustomActionTargetStringList);
        BIND(GetCustomActionTargetStringList);
        BIND(CustomActionTargetStringListContains);
        BIND(CustomActionTargetStringListContainsAny);
        BIND(CustomActionTargetStringListContainsAnyCSV);
        BIND(CustomActionTargetStringListContainsAll);
        BIND(CustomActionTargetStringListContainsAllCSV);
        BIND(GetCustomActionTargetStringListOverlap);
        BIND(GetCustomActionTargetStringListOverlapCSV);

        BIND(HasCustomActionPerformerInt);
        BIND(GetCustomActionPerformerInt);
        BIND(IsCustomActionPerformerInt);
        BIND(HasCustomActionPerformerFloat);
        BIND(GetCustomActionPerformerFloat);
        BIND(IsCustomActionPerformerFloat);
        BIND(HasCustomActionPerformerString);
        BIND(GetCustomActionPerformerString);
        BIND(IsCustomActionPerformerString);

        BIND(HasCustomActionPerformerIntList);
        BIND(GetCustomActionPerformerIntList);
        BIND(CustomActionPerformerIntListContains);
        BIND(CustomActionPerformerIntListContainsAny);
        BIND(CustomActionPerformerIntListContainsAnyCSV);
        BIND(CustomActionPerformerIntListContainsAll);
        BIND(CustomActionPerformerIntListContainsAllCSV);
        BIND(GetCustomActionPerformerIntListOverlap);
        BIND(GetCustomActionPerformerIntListOverlapCSV);

        BIND(HasCustomActionPerformerFloatList);
        BIND(GetCustomActionPerformerFloatList);
        BIND(CustomActionPerformerFloatListContains);
        BIND(CustomActionPerformerFloatListContainsAny);
        BIND(CustomActionPerformerFloatListContainsAnyCSV);
        BIND(CustomActionPerformerFloatListContainsAll);
        BIND(CustomActionPerformerFloatListContainsAllCSV);
        BIND(GetCustomActionPerformerFloatListOverlap);
        BIND(GetCustomActionPerformerFloatListOverlapCSV);

        BIND(HasCustomActionPerformerStringList);
        BIND(GetCustomActionPerformerStringList);
        BIND(CustomActionPerformerStringListContains);
        BIND(CustomActionPerformerStringListContainsAny);
        BIND(CustomActionPerformerStringListContainsAnyCSV);
        BIND(CustomActionPerformerStringListContainsAll);
        BIND(CustomActionPerformerStringListContainsAllCSV);
        BIND(GetCustomActionPerformerStringListOverlap);
        BIND(GetCustomActionPerformerStringListOverlapCSV);

        BIND(GetCustomSceneActorIntMin);
        BIND(GetCustomSceneActorIntMax);
        BIND(GetCustomSceneActorIntSum);
        BIND(GetCustomSceneActorIntProduct);
        BIND(GetCustomSceneActorFloatMin);
        BIND(GetCustomSceneActorFloatMax);
        BIND(GetCustomSceneActorFloatSum);
        BIND(GetCustomSceneActorFloatProduct);

        return true;
	}
}