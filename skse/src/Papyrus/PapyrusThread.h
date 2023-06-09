#pragma once

#include "Core/ThreadManager.h"
#include "Graph/GraphTable.h"

namespace PapyrusThread {
    using VM = RE::BSScript::IVirtualMachine;

    bool IsRunning(RE::StaticFunctionTag*, int threadID) {
        return OStim::ThreadManager::GetSingleton()->GetThread(threadID);
    }

    void Stop(RE::StaticFunctionTag*, int threadID) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            thread->stop();
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
            return thread->GetActor(index)->getActor().form;
        }
        return {};
    }

    int GetActorPosition(RE::StaticFunctionTag*, int threadID, RE::Actor* actor) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadID);
        if (thread) {
            return thread->getActorPosition(actor);
        }
        return -1;
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

        BIND(IsRunning);
        BIND(Stop);

        BIND(GetScene);
        BIND(NavigateTo);
        BIND(WarpTo);
        BIND(GetSpeed);
        BIND(SetSpeed);

        BIND(GetActors);
        BIND(GetActor);
        BIND(GetActorPosition);

        BIND(IsInAutoMode);
        BIND(StartAutoMode);
        BIND(StopAutoMode);

        BIND(CallEvent);

        return true;
    }
}