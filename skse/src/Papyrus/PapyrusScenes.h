#pragma once

#include "Core/ThreadManager.h"

namespace PapyrusScenes {
    using VM = RE::BSScript::IVirtualMachine;

    void AddActor(RE::StaticFunctionTag*, int64_t a_threadId, RE::Actor* a_actor) {
        auto tm = OStim::ThreadManager::GetSingleton();
        tm->GetThread(a_threadId)->AddActor(a_actor);
    }

    void RemoveActor(RE::StaticFunctionTag*, int64_t a_threadId) {
        auto tm = OStim::ThreadManager::GetSingleton();
        tm->GetThread(a_threadId)->RemoveActor();
    }

    OStim::ThreadActor* GetActor(int64_t a_threadId, RE::Actor* a_actor) {
        auto thread = OStim::ThreadManager::GetSingleton()->GetThread(a_threadId);
        if (thread) {
            auto actor = thread->GetActor(a_actor);
            if (actor) {
                return actor;
            }
        }
        logger::error("Actor {} not found in thread {}", a_actor->GetDisplayFullName(), a_threadId);
        return nullptr;
    }

    bool Bind(VM* a_vm) {
        const auto obj = "OSANative"sv;

        BIND(AddActor);
        BIND(RemoveActor);

        return true;
    }
}  // namespace PapyrusScenes