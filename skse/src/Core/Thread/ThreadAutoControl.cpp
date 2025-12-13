#include "Core/Thread.h"

#include "Graph/GraphTable.h"
#include "MCM/MCMTable.h"
#include "Util/Constants.h"
#include "Util/VectorUtil.h"

#include <functional>
#include <map>

namespace Threading {

    // FIX: Helper struct to track gender combination for proper scene filtering
    struct GenderInfo {
        bool isLesbian = false;  // All actors are female
        bool isGay = false;      // All actors are male
    };

#pragma region util
    bool forAnyActor(Graph::Node* node, std::function<bool(Graph::GraphActor&)> condition) {
        for (Graph::GraphActor& actor : node->actors) {
            if (condition(actor)) {
                return true;
            }
        }
        return false;
    }

    bool forAllActors(Graph::Node* node, std::function<bool(Graph::GraphActor&)> condition) {
        for (Graph::GraphActor& actor : node->actors) {
            if (!condition(actor)) {
                return false;
            }
        }
        return true;
    }

    bool forAnyAction(Graph::Node* node, std::function<bool(Graph::Action::Action&)> condition) {
        for (Graph::Action::Action& action : node->actions) {
            if (condition(action)) {
                return true;
            }
        }
        return false;
    }

    // FIX: Helper function to detect gender combination for proper scene filtering
    // This uses the base actor sex (not ActorCondition.sex) to determine scene type
    // because AGENDER actors can fulfill both roles but we still want to filter for lesbian/gay scenes
    GenderInfo detectGenderCombination(std::map<int32_t, ThreadActor>& actors) {
        GenderInfo info;

        if (actors.empty()) {
            return info;
        }

        bool allFemale = true;
        bool allMale = true;

        for (auto& [index, actor] : actors) {
            if (actor.isFemale()) {
                allMale = false;
            } else {
                allFemale = false;
            }
        }

        info.isLesbian = allFemale;
        info.isGay = allMale;

        return info;
    }

    void addFurniture(std::vector<std::function<bool(Graph::Node*)>>& conditions, Furniture::FurnitureType* furnitureType) {
        std::string furnitureTypeID = furnitureType->getListType()->id;
        if (furnitureTypeID == "none") {
            conditions.push_back([&](Graph::Node* node) {
                return forAnyActor(node, [&](Graph::GraphActor& actor) {
                    return actor.hasTag("standing");
                });
            });
        } else if (furnitureTypeID == "bed") {
            conditions.push_back([&](Graph::Node* node) {
                return forAllActors(node, [&](Graph::GraphActor& actor) {
                    return !actor.hasTag("standing");
                });
            });
        }
    }

    bool checkConditions(std::vector<std::function<bool(Graph::Node*)>>& conditions, Graph::Node* node) {
        for (std::function<bool(Graph::Node*)> condition : conditions) {
            if (!condition(node)) {
                return false;
            }
        }
        return true;
    }

    Graph::Node* findNodeWithGenderFiltering(
        Graph::Node* currentNode,
        std::vector<Trait::ActorCondition> actorConditions,
        Furniture::FurnitureType* furnitureType,
        std::vector<std::function<bool(Graph::Node*)>>& baseConditions,
        GenderInfo genderInfo,
        const char* sceneTypeForLogging
    ) {
        Graph::Node* result = nullptr;

        // Try with gender filtering first if intendedSexOnly is enabled
        if (MCM::MCMTable::intendedSexOnly()) {
            std::vector<std::function<bool(Graph::Node*)>> genderConditions = baseConditions;

            if (genderInfo.isLesbian) {
                genderConditions.push_back([](Graph::Node* node) { return node->hasTag("lesbian"); });
            } else if (genderInfo.isGay) {
                genderConditions.push_back([](Graph::Node* node) { return node->hasTag("gay"); });
            } else {
                // For mixed gender scenes, explicitly exclude lesbian and gay tagged scenes
                genderConditions.push_back([](Graph::Node* node) { return !node->hasTag("lesbian") && !node->hasTag("gay"); });
            }

            // Try with gender filtering first
            if (currentNode->hasActionTag("sexual") && MCM::MCMTable::autoModeLimitToNavigationDistance()) {
                result = currentNode->getRandomNodeInRange(MCM::MCMTable::navigationDistanceMax(), actorConditions, [&genderConditions](Graph::Node* node) { return checkConditions(genderConditions, node); });
            } else {
                result = Graph::GraphTable::getRandomNode(furnitureType, actorConditions, [&genderConditions](Graph::Node* node) { return checkConditions(genderConditions, node); });
            }

            // Fallback: If no animation found with gender tags, try without gender filtering
            if (!result) {
                logger::warn("No gender-appropriate {} animation found, falling back to untagged animations", sceneTypeForLogging);
            }
        }

        // If no result yet (either gender filtering disabled or fallback needed), search without gender tags
        if (!result) {
            if (currentNode->hasActionTag("sexual") && MCM::MCMTable::autoModeLimitToNavigationDistance()) {
                result = currentNode->getRandomNodeInRange(MCM::MCMTable::navigationDistanceMax(), actorConditions, [&baseConditions](Graph::Node* node) { return checkConditions(baseConditions, node); });
            } else {
                result = Graph::GraphTable::getRandomNode(furnitureType, actorConditions, [&baseConditions](Graph::Node* node) { return checkConditions(baseConditions, node); });
            }
        }

        return result;
    }

    Graph::Node* getRandomForeplayNode(Graph::Node* currentNode, std::vector<Trait::ActorCondition> actorConditions, Furniture::FurnitureType* furnitureType, GenderInfo genderInfo) {
        std::vector<std::function<bool(Graph::Node*)>> conditions;
        conditions.push_back([&](Graph::Node* node) { return node->findAnyAction({"analsex", "tribbing", "vaginalsex"}) == -1; });
        addFurniture(conditions, furnitureType);
        conditions.push_back([&](Graph::Node* node) {
            return forAnyAction(node, [&](Graph::Action::Action& action) {
                return action.attributes->hasTag("sexual");
            });
        });

        return findNodeWithGenderFiltering(currentNode, actorConditions, furnitureType, conditions, genderInfo, "foreplay");
    }

    Graph::Node* getRandomSexNode(Graph::Node* currentNode, std::vector<Trait::ActorCondition> actorConditions, Furniture::FurnitureType* furnitureType, GenderInfo genderInfo) {
        std::vector<std::function<bool(Graph::Node*)>> conditions;
        addFurniture(conditions, furnitureType);
        return findNodeWithGenderFiltering(currentNode, actorConditions, furnitureType, conditions, genderInfo, "sex");
    }
#pragma endregion

    void Thread::evaluateAutoMode() {
        if (!playerThread) {
            startAutoMode();
            return;
        }

        if (MCM::MCMTable::useAutoModeAlways()) {
            startAutoMode();
            return;
        }

        if (m_actors.size() == 1) {
            if (MCM::MCMTable::useAutoModeSolo()) {
                startAutoMode();
            }
            return;
        }

        // TODO auto mode for dominant or submissive player
        if (MCM::MCMTable::useAutoModeVanilla()) {
            startAutoMode();
        }
    }

    void Thread::startAutoMode() {
        if (autoMode) {
            return;
        }

        if ((threadFlags & ThreadFlag::NO_AUTO_MODE) == ThreadFlag::NO_AUTO_MODE) {
            return;
        }

        if (autoModeStage == AutoModeStage::NONE) {
            if (m_actors.size() == 1) {
                autoModeStage = AutoModeStage::MAIN;
            } else {
                if (RNGUtil::chanceRoll(MCM::MCMTable::autoModeForeplayChance())) {
                    autoModeStage = AutoModeStage::FOREPLAY;
                    foreplayThreshold = RNGUtil::uniformInt(MCM::MCMTable::autoModeForeplayThresholdMin(), MCM::MCMTable::autoModeForeplayThresholdMax());
                } else {
                    autoModeStage = AutoModeStage::MAIN;
                }
                if (RNGUtil::chanceRoll(MCM::MCMTable::autoModePulloutChance())) {
                    pulloutThreshold = RNGUtil::uniformInt(MCM::MCMTable::autoModePulloutThresholdMin(), MCM::MCMTable::autoModePulloutThresholdMax());
                }
            }
        }

        if (m_currentNode && nodeQueue.empty()) {
            if (m_currentNode->hasActionTag("sexual")) {
                startAutoModeCooldown();
            } else {
                progressAutoMode();
            }
        }

        autoMode = true;
    }

    void Thread::startAutoModeCooldown() {
        autoModeCooldown = RNGUtil::uniformInt(MCM::MCMTable::autoModeAnimDurationMin(), MCM::MCMTable::autoModeAnimDurationMax());
    }

    void Thread::progressAutoMode() {
        Graph::Node* next = nullptr;
        if (m_actors.size() == 1) {
            std::string action = GetActor(0)->hasSchlong() ? "malemasturbation" : "femalemasturbation";
            next = Graph::GraphTable::getRandomNode(furnitureType, getActorConditions(), [&action](Graph::Node* node) { return node->findAction(action) != -1; });
        } else {
            // FIX: Detect gender combination based on base actor sex for proper scene filtering
            // This ensures lesbian/gay tags are applied even when actors have AGENDER conditions (e.g., futa)
            GenderInfo genderInfo = detectGenderCombination(m_actors);

            if (autoModeStage == AutoModeStage::FOREPLAY) {
                next = getRandomForeplayNode(m_currentNode, getActorConditions(), furnitureType, genderInfo);
            } else if (autoModeStage == AutoModeStage::MAIN) {
                next = getRandomSexNode(m_currentNode, getActorConditions(), furnitureType, genderInfo);
            }
        }

        if (next) {
            navigateTo(next);
        }

        startAutoModeCooldown();
    }

    void Thread::nodeChangedAutoControl() {
        if (autoMode) {
            startAutoModeCooldown();
        }
    }

    void Thread::loopAutoControl() {
        if (!nodeQueue.empty()) {
            return;
        }

        if (!playerThread || MCM::MCMTable::autoSpeedControl()) {
            if ((autoSpeedControlCooldown -= Constants::LOOP_TIME_MILLISECONDS) <= 0) {
                float stimulation = getMaxExcitement();
                int chance;
                int min = MCM::MCMTable::autoSpeedControlExcitementMin();
                int max = MCM::MCMTable::autoSpeedControlExcitementMax();
                if (stimulation < min) {
                    chance = 0;
                } else if (stimulation > max) {
                    chance = 100;
                } else {
                    chance = ((stimulation - min) * 100) / (max - min);
                }
                if (RNGUtil::chanceRoll(chance)) {
                    increaseSpeed();
                }
                autoSpeedControlCooldown = RNGUtil::uniformInt(MCM::MCMTable::autoSpeedControlIntervalMin(), MCM::MCMTable::autoSpeedControlIntervalMax());
            }
        }


        if (!autoMode) {
            return;
        }

        if (autoModeStage == AutoModeStage::PULLOUT) {
            // wait for climax
            return;
        }

        if (autoModeStage == AutoModeStage::FOREPLAY) {
            if (getMaxExcitement() > foreplayThreshold) {
                autoModeStage = AutoModeStage::MAIN;
                autoModeCooldown = 0;
            }
        }

        if (autoModeStage == AutoModeStage::MAIN && pulloutThreshold != 0) {
            float maxExcitement = 0;
            for (auto& [index, actor] : m_actors) {
                if (actor.hasSchlong() && actor.getExcitement() > maxExcitement) {
                    maxExcitement = actor.getExcitement();
                }
            }

            if (maxExcitement > pulloutThreshold) {
                autoModeStage = AutoModeStage::PULLOUT;
                pullOut();
            }
        }

        if ((autoModeCooldown -= Constants::LOOP_TIME_MILLISECONDS) < 0) {
            progressAutoMode();
        }
    }

    void Thread::setAutoModeToMainStage() {
        if (autoModeStage == AutoModeStage::MAIN) {
            return;
        }

        autoModeStage = AutoModeStage::MAIN;
    }
}