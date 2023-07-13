#include "PlayerThreadStarter.h"

#include "Core/ThreadManager.h"

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
            if (params.startingNode) {
                RE::TESObjectREFR* bed = Furniture::findBed(params.actors[0].form, MCM::MCMTable::furnitureSearchDistance(), 96.0f);
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
                std::vector<RE::TESObjectREFR*> furniture = Furniture::findFurniture(params.actors.size(), params.actors[0].form, MCM::MCMTable::furnitureSearchDistance(), 96.0f);
                if (MCM::MCMTable::selectFurniture()) {
                    std::vector<std::string> options = {"None"};
                    std::vector<RE::TESObjectREFR*> objects;
                    for (int i = 0; i < furniture.size(); i++) {
                        if (furniture[i]) {
                            options.push_back(Furniture::FurnitureTypeAPI::getDisplayName(static_cast<Furniture::FurnitureType>(i + 1)));
                            objects.push_back(furniture[i]);
                        }
                    }
                    if (!objects.empty()) {
                        GameAPI::Game::showMessageBox("Which furniture do you want to use?", options, [params, objects](unsigned int result) {
                            if (result == 0) {
                                handleActorSorting(params);
                            } else if (result > objects.size()){
                                return;
                            } else {
                                addFurniture(params, objects[result - 1]);
                            }
                        });
                        return;
                    }
                } else {
                    for (RE::TESObjectREFR* object : furniture) {
                        if (object) {
                            addFurniture(params, object);
                            return;
                        }
                    }
                }
            }
        }
        handleActorSorting(params);
    }

    void addFurniture(ThreadStartParams params, RE::TESObjectREFR* furniture) {
        params.furniture = furniture;
        handleActorSorting(params);
    }

    void handleActorSorting(ThreadStartParams params) {
        if (params.startingNode) {
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
        if (!params.startingNode) {
            std::string nodeTag = MCM::MCMTable::useIntroScenes() ? "intro" : "idle";
            Furniture::FurnitureType furnitureType = Furniture::getFurnitureType(params.furniture, false);
            switch (furnitureType) {
            case Furniture::FurnitureType::NONE:
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorConditions::create(params.actors), [&nodeTag](Graph::Node* node) { return node->hasNodeTag(nodeTag) && node->hasActorTagOnAny("standing"); });
                break;
            case Furniture::FurnitureType::BED:
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorConditions::create(params.actors), [&nodeTag](Graph::Node* node) { return node->hasNodeTag(nodeTag) && !node->hasActorTagOnAny("standing"); });
                break;
            default:
                params.startingNode = Graph::GraphTable::getRandomNode(furnitureType, Trait::ActorConditions::create(params.actors), [&nodeTag](Graph::Node* node) { return node->hasNodeTag(nodeTag); });
                break;
            }
        }

        if (!params.startingNode) {
            logger::info("no starting node found");
            return;
        }

        startInner(params);
    }

    void startInner(ThreadStartParams params) {
        if (MCM::MCMTable::useFades()) {
            std::thread fadeThread = std::thread([params] {
                GameAPI::GameCamera::fadeToBlack(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(700));
                ThreadManager::GetSingleton()->startThread(params);
                std::this_thread::sleep_for(std::chrono::milliseconds(550));
                GameAPI::GameCamera::fadeFromBlack(1);
            });
            fadeThread.detach();
        } else {
            ThreadManager::GetSingleton()->startThread(params);
        }
    }
}