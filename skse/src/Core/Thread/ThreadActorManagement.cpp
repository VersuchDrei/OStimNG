#include "../Thread.h"

#include "Core/ThreadManager.h"
#include "GameAPI/Game.h"
#include "GameAPI/GameCamera.h"
#include "GameAPI/GameEvents.h"
#include "Graph/GraphTable.h"
#include "MCM/MCMTable.h"
#include "Util/ActorUtil.h"

namespace Threading {
    namespace {
        // Helper function to sort actors in thread order
        void sortThreadActors(Thread* thread) {
            // Build actor vector from thread
            std::vector<GameAPI::GameActor> actorVector;
            for (auto& [position, threadActor] : thread->getActors()) {
                actorVector.push_back(threadActor.getActor());
            }

            // Sort using OStim's standard sorting (no dominant actors, auto player position)
            std::vector<GameAPI::GameActor> dominantActors; // Empty - no explicit dominants
            ActorUtil::sort(actorVector, dominantActors, -1); // -1 = auto player position

            // Rebuild the actor map with sorted order
            std::map<int32_t, ThreadActor> sortedActors;
            for (int i = 0; i < actorVector.size(); i++) {
                // Find the ThreadActor for this actor
                for (auto& [oldPos, threadActor] : thread->getActors()) {
                    if (threadActor.getActor() == actorVector[i]) {
                        // Create copy with updated index
                        ThreadActor sorted = threadActor;
                        sorted.index = i;
                        sortedActors.insert({i, sorted});
                        break;
                    }
                }
            }

            // Replace the thread's actor map
            thread->getActors() = sortedActors;
        }
    }

    bool Thread::canAddActor(RE::Actor* actor) {
        if (!actor) {
            logger::info("Cannot add actor: null actor pointer");
            return false;
        }

        std::string actorName = GameAPI::GameActor(actor).getName();

        // Check if actor is already in thread
        if (GetActor(GameAPI::GameActor(actor)) != nullptr) {
            logger::info("Cannot add actor {}: actor already in thread", actorName);
            return false;
        }

        // Check if actor is locked by another thread
        if (ThreadManager::GetSingleton()->findThread(GameAPI::GameActor(actor))) {
            logger::info("Cannot add actor {}: actor is in another thread", actorName);
            return false;
        }

        // Build actor vector with the new actor to test sorting
        std::vector<GameAPI::GameActor> testActors;
        for (auto& [position, threadActor] : m_actors) {
            testActors.push_back(threadActor.getActor());
        }
        testActors.push_back(GameAPI::GameActor(actor));

        // Sort the test actors (this is what will happen in addActorToThreadInner)
        std::vector<GameAPI::GameActor> dominantActors; // Empty - no explicit dominants
        ActorUtil::sort(testActors, dominantActors, -1); // -1 = auto player position

        // Build conditions in the sorted order
        std::vector<Trait::ActorCondition> conditions;
        for (GameAPI::GameActor& sortedActor : testActors) {
            conditions.push_back(Trait::ActorCondition::create(sortedActor.form));
        }

        // Check if a compatible node exists for the sorted actor configuration
        Graph::Node* testNode = Graph::GraphTable::getRandomNode(
            furnitureType,
            conditions,
            [](Graph::Node* node) { return !node->isTransition; });

        if (!testNode) {
            logger::info("Cannot add actor {}: no compatible node found for {} actors", actorName, conditions.size());
            return false;
        }

        return true;
    }

    void Thread::addActorToThreadInner(RE::Actor* actor) {
        // Validate we can add this actor
        if (!canAddActor(actor)) {
            logger::warn("Cannot add actor: validation failed");
            return;
        }

        logger::info("Adding actor to thread: {}", GameAPI::GameActor(actor).getName());

        // Store current speed to restore if possible
        int currentSpeed = m_currentNodeSpeed;

        // Determine new position (end of actor list)
        int newPosition = m_actors.size();

        // Add actor using existing infrastructure
        AddActor(actor);

        // Sort actors in proper order (dominant/schlong first, player position)
        sortThreadActors(this);

        // Rebuild actor conditions with new actor (now in sorted order)
        std::vector<Trait::ActorCondition> conditions = getActorConditions();

        // Find compatible node for new actor count (canAddActor already validated one exists)
        Graph::Node* targetNode = Graph::GraphTable::getRandomNode(
            furnitureType,
            conditions,
            [](Graph::Node* node) { return !node->isTransition; });

        // Rebuild alignment key with new actor count
        rebuildAlignmentKey();

        // Change to new node
        ChangeNode(targetNode);

        // Try to restore speed if the new node supports it
        if (m_currentNode && currentSpeed < m_currentNode->speeds.size()) {
            SetSpeed(currentSpeed);
        }

        // Send event notification
        GameAPI::GameEvents::sendActorAddedEvent(m_threadId, newPosition);

        logger::info("Successfully added actor at position {}", newPosition);
    }

    bool Thread::addActorToThread(RE::Actor* actor, bool useFades) {
        // Validate we can add this actor
        if (!canAddActor(actor)) {
            logger::warn("Cannot add actor: validation failed");
            return false;
        }

        if (playerThread && useFades && MCM::MCMTable::useFades()) {
            std::thread fadeThread = std::thread([this, actor] {
                GameAPI::GameCamera::fadeToBlack(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(700));
                Thread* thread = ThreadManager::GetSingleton()->GetThread(m_threadId);
                if (thread) {
                    thread->addActorToThreadInner(actor);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(550));
                GameAPI::GameCamera::fadeFromBlack(1);
            });
            fadeThread.detach();
        } else {
            addActorToThreadInner(actor);
        }

        return true;
    }

    void Thread::addActorWithUI() {
        logger::info("addActorWithUI called, current actor count: {}", m_actors.size());

        // Get nearby actors from player's perspective
        std::vector<GameAPI::GameActor> nearbyActors = GameAPI::GameActor::getPlayer().getNearbyActors(2000.0f, [this](GameAPI::GameActor actor) {
            return canAddActor(actor.form);
        });

        // Convert to RE::Actor* vector
        std::vector<RE::Actor*> validActors;
        for (GameAPI::GameActor& gameActor : nearbyActors) {
            if (gameActor.form) {
                validActors.push_back(gameActor.form);
            }
        }

        if (validActors.empty()) {
            GameAPI::Game::notification("No valid actors nearby to add");
            logger::info("No valid actors found nearby");
            return;
        }

        // Build message box options
        std::vector<std::string> actorNames = {"$ostim_message_none"};
        for (RE::Actor* actor : validActors) {
            actorNames.push_back(GameAPI::GameActor(actor).getName());
        }

        // Capture thread ID for async callback
        ThreadId threadId = m_threadId;

        // Show message box to select actor
        GameAPI::Game::showMessageBox("$ostim_message_add_actor", actorNames,
            [threadId, validActors](unsigned int result) {
                if (result == 0 || result > validActors.size()) {
                    // User cancelled or invalid selection
                    return;
                }

                // Get thread again (important for async safety)
                Thread* thread = ThreadManager::GetSingleton()->GetThread(threadId);
                if (!thread) {
                    return;
                }

                // Add the selected actor
                RE::Actor* selectedActor = validActors[result - 1];
                bool success = thread->addActorToThread(selectedActor);

                if (success) {
                    GameAPI::Game::notification("Actor added successfully");
                } else {
                    GameAPI::Game::notification("Failed to add actor");
                }
            });
    }

    bool Thread::canRemoveActor(int position) {
        // Check if position exists
        if (m_actors.find(position) == m_actors.end()) {
            logger::info("Cannot remove actor: position {} doesn't exist", position);
            return false;
        }

        // Ensure at least 1 actor remains (minimum for a thread)
        if (m_actors.size() <= 1) {
            logger::info("Cannot remove actor: would leave thread with no actors");
            return false;
        }

        // Build actor vector without the removed actor to test sorting
        std::vector<GameAPI::GameActor> testActors;
        for (auto& [idx, threadActor] : m_actors) {
            if (idx != position) {
                testActors.push_back(threadActor.getActor());
            }
        }

        // Sort the remaining actors (this is what will happen in removeActorFromThreadInner)
        std::vector<GameAPI::GameActor> dominantActors; // Empty - no explicit dominants
        ActorUtil::sort(testActors, dominantActors, -1); // -1 = auto player position

        // Build conditions in the sorted order
        std::vector<Trait::ActorCondition> conditions;
        for (GameAPI::GameActor& sortedActor : testActors) {
            conditions.push_back(Trait::ActorCondition::create(sortedActor.form));
        }

        // Check if a compatible node exists for the sorted remaining actors
        Graph::Node* testNode = Graph::GraphTable::getRandomNode(
            furnitureType,
            conditions,
            [](Graph::Node* node) { return !node->isTransition; });

        if (!testNode) {
            logger::info("Cannot remove actor: no compatible node found for {} actors", conditions.size());
            return false;
        }

        return true;
    }

    void Thread::removeActorFromThreadInner(int position) {
        logger::info("Removing actor from thread at position: {}", position);

        // Store current speed to restore if possible
        int currentSpeed = m_currentNodeSpeed;

        // Get actor to remove
        ThreadActor* actorToRemove = GetActor(position);
        if (!actorToRemove) {
            logger::warn("Failed to remove actor: null actor pointer at position {}", position);
            return;
        }

        // Free actor resources
        removeActorSink(actorToRemove->getActor().form);
        actorToRemove->free();

        // Remove from map
        m_actors.erase(position);

        // CRITICAL: Reindex remaining actors to fill the gap
        // If we remove position 1 from [0,1,2,3], we need to shift to [0,1,2]
        std::map<int32_t, ThreadActor> reindexedActors;
        int newIndex = 0;
        for (auto& [oldIndex, actor] : m_actors) {
            ThreadActor temp = actor;
            temp.index = newIndex;
            reindexedActors.insert({newIndex, temp});
            newIndex++;
        }
        m_actors = reindexedActors;

        // Sort actors in proper order (dominant/schlong first, player position)
        sortThreadActors(this);

        // Rebuild actor conditions (now in sorted order)
        std::vector<Trait::ActorCondition> conditions = getActorConditions();

        // Find compatible node for new actor count
        Graph::Node* targetNode = Graph::GraphTable::getRandomNode(
            furnitureType,
            conditions,
            [](Graph::Node* node) { return !node->isTransition; });

        if (!targetNode) {
            logger::error("Failed to find compatible node after removing actor - stopping thread");
            // At this point, actor is already removed - we're in a bad state
            // Best option: stop the thread gracefully
            stop();
            return;
        }

        // Rebuild alignment key
        rebuildAlignmentKey();

        // Change to new node
        ChangeNode(targetNode);

        // Try to restore speed if the new node supports it
        if (m_currentNode && currentSpeed < m_currentNode->speeds.size()) {
            SetSpeed(currentSpeed);
        }

        // Send event notification
        GameAPI::GameEvents::sendActorRemovedEvent(m_threadId, position);

        logger::info("Successfully removed actor from position {}", position);
    }

    bool Thread::removeActorFromThread(int position, bool useFades) {
        if (!canRemoveActor(position)) {
            return false;
        }

        if (playerThread && useFades && MCM::MCMTable::useFades()) {
            std::thread fadeThread = std::thread([this, position] {
                GameAPI::GameCamera::fadeToBlack(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(700));
                Thread* thread = ThreadManager::GetSingleton()->GetThread(m_threadId);
                if (thread) {
                    thread->removeActorFromThreadInner(position);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(550));
                GameAPI::GameCamera::fadeFromBlack(1);
            });
            fadeThread.detach();
        } else {
            removeActorFromThreadInner(position);
        }

        return true;
    }

    void Thread::removeActorWithUI() {
        logger::info("removeActorWithUI called, actor count: {}", m_actors.size());

        // Check if thread has at least 2 actors (need to keep at least 1)
        if (m_actors.size() <= 1) {
            logger::warn("Cannot remove actor: only {} actor(s) in thread", m_actors.size());
            GameAPI::Game::notification("Cannot remove actor: minimum 1 actor required");
            return;
        }

        // Build list of removable actors
        std::vector<std::string> actorNames = {"$ostim_message_none"};
        std::vector<int> actorPositions;

        int maxOptions = GameAPI::Game::getMessageBoxOptionLimit();
        int count = 0;
        for (auto& [position, actor] : m_actors) {
            if (count >= maxOptions) {
                break;
            }
            if (canRemoveActor(position)) {
                // Include position in name for clarity
                std::string displayName = actor.getActor().getName() + " (Position " + std::to_string(position) + ")";
                actorNames.push_back(displayName);
                actorPositions.push_back(position);
                count++;
            }
        }

        if (actorPositions.empty()) {
            GameAPI::Game::notification("Cannot remove any actors from this scene");
            logger::info("No removable actors found");
            return;
        }

        // Capture thread ID for async callback
        ThreadId threadId = m_threadId;

        // Show message box to select actor to remove
        GameAPI::Game::showMessageBox("$ostim_message_remove_actor", actorNames,
            [threadId, actorPositions](unsigned int result) {
                if (result == 0 || result > actorPositions.size()) {
                    // User cancelled or invalid selection
                    return;
                }

                // Get thread again (important for async safety)
                Thread* thread = ThreadManager::GetSingleton()->GetThread(threadId);
                if (!thread) {
                    return;
                }

                // Remove the selected actor
                int selectedPosition = actorPositions[result - 1];
                bool success = thread->removeActorFromThread(selectedPosition);

                if (success) {
                    GameAPI::Game::notification("Actor removed successfully");
                } else {
                    GameAPI::Game::notification("Failed to remove actor");
                }
            });
    }
}
