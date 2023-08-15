#include "PlayerThreadStarter.h"

#include "Core/ThreadManager.h"

#include "Furniture/FurnitureTable.h"
#include "GameAPI/Game.h"
#include "GameAPI/GameCamera.h"
#include "Graph/GraphTable.h"
#include "MCM/MCMTable.h"
#include "Util/ActorUtil.h"
#include "Util/CompatibilityTable.h"

namespace OStim {
    std::vector<std::string> positions{"Dom", "Sub", "Third", "Fourth", "Fifth", "Sixth", "Seventh", "Eigth", "Ninth"};

    void startPlayerThread(ThreadStartParams params) {
        std::thread startThread = std::thread([params] {
            bool inDialogue = getInDialogue(params.actors);
            while (inDialogue) {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                inDialogue = getInDialogue(params.actors);
            }

            handleFurniture(params);
        });
        startThread.detach();
    }

    bool getInDialogue(std::vector<GameAPI::GameActor> actors) {
        for (GameAPI::GameActor actor : actors) {
            if (actor.isInDialogue()) {
                return true;
            }
        }
        return false;
    }

    void handleFurniture(ThreadStartParams params) {
        if (!params.noFurniture && !params.furniture && MCM::MCMTable::useFurniture()) {
            if (params.startingNode || params.startingSequence) {
                GameAPI::GameObject bed = Furniture::findBed(params.actors[0].form, MCM::MCMTable::furnitureSearchDistance(), 96.0f);
                if (bed) {
                    if (MCM::MCMTable::selectFurniture()) {
                        GameAPI::Game::showMessageBox("Do you want to use the nearby bed?", {"Yes", "No"}, [params, bed](unsigned int result) {
                            if (result == 0) {
                                addFurniture(params, bed);
                            } else {
                                handleActorSorting(params);
                            }
                        });
                        return;
                    } else {
                        addFurniture(params, bed);
                        return;
                    }
                }
            } else {
                std::vector<std::pair<Furniture::FurnitureType*, GameAPI::GameObject>> furniture = Furniture::findFurniture(params.actors.size(), params.actors[0].form, MCM::MCMTable::furnitureSearchDistance(), 96.0f);
                if (MCM::MCMTable::selectFurniture()) {
                    std::vector<std::string> options = {"None"};
                    std::vector<GameAPI::GameObject> objects;
                    int max = std::min(static_cast<int>(furniture.size()), GameAPI::Game::getMessageBoxOptionLimit() - 1);
                    for (int i = 0; i < furniture.size(); i++) {
                        options.push_back(furniture[i].first->name);
                        objects.push_back(furniture[i].second);
                    }
                    if (!objects.empty()) {
                        GameAPI::Game::showMessageBox("Which furniture do you want to use?", options, [params, objects](unsigned int result) {
                            if (result == 0) {
                                handleActorAdding(params);
                            } else if (result > objects.size()){
                                return;
                            } else {
                                addFurniture(params, objects[result - 1]);
                            }
                        });
                        return;
                    }
                } else {
                    if (!furniture.empty()) {
                        addFurniture(params, furniture.front().second);
                    }
                }
            }
        }
        handleActorAdding(params);
    }

    void addFurniture(ThreadStartParams params, GameAPI::GameObject furniture) {
        params.furniture = furniture;
        handleActorAdding(params);
    }

    void handleActorAdding(ThreadStartParams params) {
        if (params.startingNode || params.startingSequence) {
            startInner(params);
            return;
        }

        if (!MCM::MCMTable::addActorsAtStart()) {
            handleActorSorting(params);
            return;
        }

        handleActorAddingInner(params);
    }

    void handleActorAddingInner(ThreadStartParams params) {
        if (!Graph::GraphTable::hasNodes(Furniture::FurnitureTable::getFurnitureType(params.furniture, false), params.actors.size() + 1)) {
            handleActorSorting(params);
            return;
        }

        std::vector<GameAPI::GameActor> actors = GameAPI::GameActor::getPlayer().getNearbyActors(2000, [&params](GameAPI::GameActor actor){
            return !VectorUtil::contains(params.actors, actor) && OStim::isEligible(actor);    
        });

        if (actors.empty()) {
            handleActorSorting(params);
            return;
        }

        std::vector<std::string> options;

        options.push_back("None");
        int max = std::min<int>(GameAPI::Game::getMessageBoxOptionLimit() - 1, actors.size());
        int i = 0;
        while (i < max) {
            options.push_back(actors[i].getName());
            i++;
        }

        GameAPI::Game::showMessageBox("Add actor?", options, [params, actors](unsigned int result) {
            if (result > 0) {
                addActor(params, actors[result - 1]);
            } else {
                handleActorSorting(params);
            }
        });
    }

    void addActor(ThreadStartParams params, GameAPI::GameActor actor) {
        params.actors.push_back(actor);
        handleActorAddingInner(params);
    }

    void handleActorSorting(ThreadStartParams params) {
        if (params.startingNode || params.startingSequence) {
            startInner(params);
            return;
        }

        bool selectRole = false;
        if (params.actors.size() >= 3) {
            selectRole = MCM::MCMTable::playerSelectRoleThreesome();
        } else if (params.actors.size() == 2) {
            if (Compatibility::CompatibilityTable::hasSchlong(params.actors[0]) == Compatibility::CompatibilityTable::hasSchlong(params.actors[1])) {
                selectRole = MCM::MCMTable::playerSelectRoleGay();
            } else {
                selectRole = MCM::MCMTable::playerSelectRoleStraight();
            }
        }

        if (selectRole) {
            std::vector<std::string> options;
            for (int i = 0; i < params.actors.size(); i++) {
                options.push_back(positions[i]);
            }
            GameAPI::Game::showMessageBox("Which position do you want to be in?", options, [params](unsigned int result) {
                sortActors(params, result);
            });
        } else {
            sortActors(params, -1);
        }
    }

    void sortActors(ThreadStartParams params, int playerIndex) {
        ActorUtil::sort(params.actors, params.dominantActors, playerIndex);
        handleStartingNode(params);
    }

    void handleStartingNode(ThreadStartParams params) {
        if (!params.startingNode && !params.startingSequence) {
            std::string nodeTag = MCM::MCMTable::useIntroScenes() ? "intro" : "idle";
            Furniture::FurnitureType* furnitureType = Furniture::FurnitureTable::getFurnitureType(params.furniture, false);
            std::string furnitureTypeID = furnitureType->getListType()->id;

            if (furnitureTypeID == "none") {
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorCondition::create(params.actors), [&nodeTag](Graph::Node* node) { return node->hasNodeTag(nodeTag) && node->hasActorTagOnAny("standing"); });
            } else if (furnitureTypeID == "bed") {
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorCondition::create(params.actors), [&nodeTag](Graph::Node* node) { return node->hasNodeTag(nodeTag) && !node->hasActorTagOnAny("standing"); });
            } else {
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorCondition::create(params.actors), [&nodeTag](Graph::Node* node) { return node->hasNodeTag(nodeTag); });
            }
        }
        
        if (!params.startingNode && !params.startingSequence) {
            GameAPI::Game::notification("no starting animation found");
            return;
        }

        startInner(params);
    }

    void startInner(ThreadStartParams params) {
        logger::info("starting scene");
        if (MCM::MCMTable::useFades()) {
            std::thread fadeThread = std::thread([params] {
                GameAPI::GameCamera::fadeToBlack(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(700));
                GameAPI::Game::runSynced([params] { ThreadManager::GetSingleton()->startThread(params); });
                std::this_thread::sleep_for(std::chrono::milliseconds(550));
                GameAPI::GameCamera::fadeFromBlack(1);
            });
            fadeThread.detach();
        } else {
            ThreadManager::GetSingleton()->startThread(params);
        }
    }
}