#pragma once

#include "Core/ThreadManager.h"

namespace PapyrusScenes {
    using VM = RE::BSScript::IVirtualMachine;

    void StartScene(RE::StaticFunctionTag*, int64_t id, RE::TESObjectREFR* furniture, std::vector<RE::Actor*> actors) {
        auto tm = OStim::ThreadManager::GetSingleton();
        tm->TrackThread(id, furniture, actors);
    }

    void EndScene(RE::StaticFunctionTag*, int64_t a_id) {
        auto tm = OStim::ThreadManager::GetSingleton();
        tm->UnTrackThread(a_id);
    }

    void ChangeAnimation(RE::StaticFunctionTag*, int64_t a_id, std::string a_nodeName) {
        auto tm = OStim::ThreadManager::GetSingleton();
        auto log = RE::ConsoleLog::GetSingleton();
        auto thread = tm->GetThread(a_id);
        if (thread) {
            auto node = Graph::LookupTable::getNodeById(a_nodeName);
            if (node) {
                thread->ChangeNode(node);
                log->Print(("Changing Animation on thread: " + std::to_string(a_id) + " to " + a_nodeName).c_str());

            } else {
                log->Print(("Node " + a_nodeName + " not found").c_str());
            }

        } else {
            log->Print(("Thread "+ std::to_string(a_id) + "not found").c_str());
        }
    }

    void AddActor(RE::StaticFunctionTag*, int64_t a_threadId, RE::Actor* a_actor) {
        auto tm = OStim::ThreadManager::GetSingleton();
        tm->GetThread(a_threadId)->AddActor(a_actor);
    }

    void RemoveActor(RE::StaticFunctionTag*, int64_t a_threadId) {
        auto tm = OStim::ThreadManager::GetSingleton();
        tm->GetThread(a_threadId)->RemoveActor();
    }

    void UpdateSpeed(RE::StaticFunctionTag*, int64_t a_threadId, int a_speed){
        auto tm = OStim::ThreadManager::GetSingleton();
        auto thread = tm->GetThread(a_threadId);
        if(thread) thread->SetSpeed(a_speed);
    }

    OStim::ThreadActor* GetActor(int64_t a_threadId, RE::Actor* a_actor) {
        auto thread = OStim::ThreadManager::GetSingleton()->GetThread(a_threadId);
        if (thread) {
            auto actor = thread->GetActor(a_actor);
            if (actor) {
                return actor;
            }
        }
        logger::error("Actor {} not found in scene", a_actor->GetDisplayFullName(), a_threadId);
        return nullptr;
    }    

    float GetActorExcitement(RE::StaticFunctionTag*, int64_t a_threadId, RE::Actor* a_actor) {
        auto actor = GetActor(a_threadId, a_actor);
        if (actor) {
            return actor->excitement;
        }
        
        return 0;
    }

    void SetActorExcitement(RE::StaticFunctionTag*, int64_t a_threadId, RE::Actor* a_actor, float a_excitement) {
        auto actor = GetActor(a_threadId, a_actor);
        if (actor) {
            actor->excitement = a_excitement;
        }
    }

    bool Bind(VM* a_vm) {
        const auto obj = "OSANative"sv;

        BIND(StartScene);
        BIND(EndScene);
        BIND(ChangeAnimation);
        BIND(AddActor);
        BIND(RemoveActor);
        BIND(UpdateSpeed);
        BIND(GetActorExcitement);
        BIND(SetActorExcitement);

        return true;
    }
}  // namespace PapyrusScenes