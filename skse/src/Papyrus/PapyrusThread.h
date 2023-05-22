#pragma once

#include "Core/ThreadManager.h"

namespace PapyrusThread {
    using VM = RE::BSScript::IVirtualMachine;

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

        BIND(GetScene);

        BIND(GetActors);
        BIND(GetActor);
        BIND(GetActorPosition);

        BIND(CallEvent);

        return true;
    }
}