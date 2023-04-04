#pragma once

#include "Core/ThreadManager.h"

namespace PapyrusThread {
    using VM = RE::BSScript::IVirtualMachine;

    std::string GetScene(RE::StaticFunctionTag*, int threadId) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadId);
        if (thread) {
            Graph::Node* node = thread->getCurrentNode();
            if (node) {
                return node->scene_id;
            }
        }
        return "";
    }

    int GetActorPosition(RE::StaticFunctionTag*, int threadId, RE::Actor* actor) {
        OStim::Thread* thread = OStim::ThreadManager::GetSingleton()->GetThread(threadId);
        if (thread) {
            return thread->getActorPosition(actor);
        }
        return -1;
    }

    bool Bind(VM* a_vm) {
        const auto obj = "OThread"sv;

        BIND(GetScene);
        BIND(GetActorPosition);

        return true;
    }
}