#pragma once

#include "Core/ThreadStarter/ThreadStarter.h"
#include "Core/ThreadManager.h"
#include "Graph/GraphTable.h"
#include "Util/StringUtil.h"

namespace PapyrusThread {
    using VM = RE::BSScript::IVirtualMachine;

    int QuickStart(RE::StaticFunctionTag*, std::vector<RE::Actor*> actors, std::string startingAnimation, RE::TESObjectREFR* furniture) {
        OStim::ThreadStartParams params;
        params.actors = GameAPI::GameActor::convertVector(actors);
        params.startingNode = Graph::GraphTable::getNodeById(startingAnimation);
        params.furniture = furniture;

        return OStim::startThread(params);
    }

    bool IsRunning(RE::StaticFunctionTag*, int threadID) {
        return OStim::ThreadManager::GetSingleton()->GetThread(threadID);
    }

    void Stop(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            thread->stopFaded();
        }
    }

    std::string GetScene(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            Graph::Node* node = thread->getCurrentNode();
            if (node) {
                return node->scene_id;
            }
        }
        return "";
    }

    void NavigateTo(RE::StaticFunctionTag*, int threadID, std::string sceneID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        Graph::Node* node = Graph::GraphTable::getNodeById(sceneID);
        if (thread && node) {
            thread->navigateTo(node);
        }
    }

    void WarpTo(RE::StaticFunctionTag*, int threadID, std::string sceneID, bool useFades) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        Graph::Node* node = Graph::GraphTable::getNodeById(sceneID);
        if (thread && node) {
            thread->warpTo(node, useFades);
        }
    }

    bool AutoTransition(RE::StaticFunctionTag*, int threadID, std::string type) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->autoTransition(type);
        }
        return false;
    }

    bool AutoTransitionForActor(RE::StaticFunctionTag*, int threadID, int index, std::string type) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->autoTransition(index, type);
        }
        return false;
    }

    int GetSpeed(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->getCurrentSpeed();
        }
        return -1;
    }

    void SetSpeed(RE::StaticFunctionTag*, int threadID, int speed) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            thread->SetSpeed(speed);
        }
    }


    std::vector<RE::Actor*> GetActors(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            std::vector<RE::Actor*> actors = std::vector<RE::Actor*>(thread->getActors().size());
            for (auto& [index, actor] : thread->getActors()) {
                actors[index] = actor.getActor().form;
            }
            return actors;
        }
        return {};
    }

    RE::Actor* GetActor(RE::StaticFunctionTag*, int threadID, int index) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            OStim::ThreadActor* actor = thread->GetActor(index);
            if (actor) {
                return actor->getActor().form;
            }
        }
        return nullptr;
    }

    int GetActorPosition(RE::StaticFunctionTag*, int threadID, RE::Actor* actor) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->getActorPosition(actor);
        }
        return -1;
    }


    void StallClimax(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            thread->setStallClimax(true);
        }
    }

    void PermitClimax(RE::StaticFunctionTag*, int threadID, bool permitActors) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            thread->setStallClimax(false);
            if (permitActors) {
                for (auto& [index, actor] : thread->getActors()) {
                    actor.setStallClimax(false);
                }
            }
        }
    }

    bool IsClimaxStalled(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->getStallClimax();
        }
        return false;
    }


    RE::TESObjectREFR* GetFurniture(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->getFurniture().form;
        }
        return nullptr;
    }

    std::string GetFurnitureType(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->getFurnitureType()->getListType()->id;
        }
        return "";
    }


    bool IsInAutoMode(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->isInAutoMode();
        }
        return false;
    }

    void StartAutoMode(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            thread->startAutoMode();
        }
    }

    void StopAutoMode(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            thread->stopAutoMode();
        }
    }


    void AddMetadata(RE::StaticFunctionTag*, int threadID, std::string metadata) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            thread->addMetadata(metadata);
        }
    }

    bool HasMetadata(RE::StaticFunctionTag*, int threadID, std::string metadata) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->hasMetadata(metadata);
        }
        return false;
    }

    std::vector<std::string> GetMetadata(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->getMetadata();
        }
        return {};
    }


    void CallEvent(RE::StaticFunctionTag*, int threadID, std::string eventName, int actor, int target, int performer) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            int size = thread->getActors().size(); // need to convert to unsigned int before comparing or shit goes sideways
            if (actor < 0 || actor >= size || target >= size || performer >= size) {
                return;
            }
            if (target < 0) {
                target = actor;
            }
            if (performer < 0) {
                performer = actor;
            }

            thread->callEvent(eventName, actor, target, performer);
        }
    }


    bool Bind(VM* a_vm) {
        const auto obj = "OThread"sv;

        BIND(QuickStart);

        BIND(IsRunning);
        BIND(Stop);

        BIND(GetScene);
        BIND(NavigateTo);
        BIND(WarpTo);
        BIND(AutoTransition);
        BIND(AutoTransitionForActor);
        BIND(GetSpeed);
        BIND(SetSpeed);

        BIND(GetActors);
        BIND(GetActor);
        BIND(GetActorPosition);

        BIND(StallClimax);
        BIND(PermitClimax);
        BIND(IsClimaxStalled);

        BIND(GetFurniture);
        BIND(GetFurnitureType);

        BIND(IsInAutoMode);
        BIND(StartAutoMode);
        BIND(StopAutoMode);

        BIND(AddMetadata);
        BIND(HasMetadata);
        BIND(GetMetadata);

        BIND(CallEvent);

        return true;
    }
}