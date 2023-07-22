#include "Core/Thread.h"

#include "Core/ThreadManager.h"

#include "GameAPI/GameCamera.h"
#include "Graph/GraphTable.h"
#include "Util/VectorUtil.h"

namespace OStim {

#pragma region util
    // TODO: move this to a ThreadUtil file?
    bool forAnyActor2(Graph::Node* node, std::function<bool(Graph::GraphActor&)> condition) {
        for (Graph::GraphActor& actor : node->actors) {
            if (condition(actor)) {
                return true;
            }
        }
        return false;
    }

    bool forAllActors2(Graph::Node* node, std::function<bool(Graph::GraphActor&)> condition) {
        for (Graph::GraphActor& actor : node->actors) {
            if (!condition(actor)) {
                return false;
            }
        }
        return true;
    }

    void addFurniture2(std::vector<std::function<bool(Graph::Node*)>>& conditions, Furniture::FurnitureType furnitureType) {
        if (furnitureType == Furniture::FurnitureType::NONE) {
            conditions.push_back([&](Graph::Node* node) {
                return forAnyActor2(node, [&](Graph::GraphActor& actor) {
                    return VectorUtil::contains(actor.tags, std::string("standing"));
                });
            });
        } else if (furnitureType == Furniture::FurnitureType::BED) {
            conditions.push_back([&](Graph::Node* node) {
                return forAllActors2(node, [&](Graph::GraphActor& actor) {
                    return !VectorUtil::contains(actor.tags, std::string("standing"));
                });
            });
        }
    }

    bool checkConditions2(std::vector<std::function<bool(Graph::Node*)>>& conditions, Graph::Node* node) {
        for (std::function<bool(Graph::Node*)> condition : conditions) {
            if (!condition(node)) {
                return false;
            }
        }
        return true;
    }
#pragma endregion

    bool Thread::autoTransition(std::string type) {
        if (!nodeQueue.empty()) {
            return false;
        }

        std::string sceneID = m_currentNode->getAutoTransitionForNode(type);
        if (sceneID.empty()) {
            return false;
        }

        Graph::Node* node = Graph::GraphTable::getNodeById(sceneID);
        if (!node) {
            return false;
        }

        ChangeNode(node);
        return true;
    }

    bool Thread::autoTransition(int index, std::string type) {
        if (!nodeQueue.empty()) {
            return false;
        }

        std::string sceneID = m_currentNode->getAutoTransitionForActor(index, type);
        if (sceneID.empty()) {
            return false;
        }

        Graph::Node* node = Graph::GraphTable::getNodeById(sceneID);
        if (!node) {
            return false;
        }

        ChangeNode(node);
        return true;
    }

    void Thread::warpTo(Graph::Node* node, bool useFades) {
        clearNodeQueue();

        if (playerThread && useFades) {
            std::thread fadeThread = std::thread([node] {
                GameAPI::GameCamera::fadeToBlack(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(700));
                Thread* thread = ThreadManager::GetSingleton()->getPlayerThread();
                if (thread) {
                    thread->ChangeNode(node);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(550));
                GameAPI::GameCamera::fadeFromBlack(1);
            });
            fadeThread.detach();
        } else {
            ChangeNode(node);
        }
    }

    void Thread::navigateTo(Graph::Node* node) {
        clearNodeQueue();

        std::vector<Graph::Node*> nodes = m_currentNode->getRoute(MCM::MCMTable::navigationDistanceMax(), getActorConditions(), node);
        if (nodes.empty()) {
            warpTo(node, MCM::MCMTable::useAutoModeFades());
        } else {
            for (int i = 1; i < nodes.size(); i++) {
                nodeQueue.push(nodes[i]);
            }
            ChangeNode(nodes.front());
        }
    }

    bool Thread::pullOut() {
        if (autoTransition("pullout")) {
            return true;
        }

        std::vector<std::function<bool(Graph::Node*)>> conditions;
        conditions.push_back([&](Graph::Node* node) { return node->findAnyAction({"analsex", "tribbing", "vaginalsex"}) == -1; });
        conditions.push_back([&](Graph::Node* node) { return node->findAction("malemasturbation") != -1; });

        Graph::Node* node = m_currentNode->getRandomNodeInRange(3, getActorConditions(), [&conditions](Graph::Node* node) { return checkConditions2(conditions, node); });

        if (node) {
            navigateTo(node);
            return true;
        }

        return false;
    }

    void Thread::clearNodeQueue() {
        while (!nodeQueue.empty()) {
            nodeQueue.pop();
        }
    }
}