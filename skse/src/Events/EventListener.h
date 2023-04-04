#pragma once

#include "Core/Singleton.h"
#include "Core/ThreadManager.h"
#include "Graph/LookupTable.h"
#include "Util/MCMTable.h"

namespace Events {
    class EventListener final : public OStim::ISingleton<EventListener>, public RE::BSTEventSink<RE::TESLoadGameEvent>, public RE::BSTEventSink<SKSE::NiNodeUpdateEvent> {
    public:
        virtual RE::BSEventNotifyControl ProcessEvent(const RE::TESLoadGameEvent* a_event, RE::BSTEventSource<RE::TESLoadGameEvent>* a_eventSource) override;
        virtual RE::BSEventNotifyControl ProcessEvent(const SKSE::NiNodeUpdateEvent* a_event, RE::BSTEventSource<SKSE::NiNodeUpdateEvent>* a_eventSource) override;

    private:
        class CheckPapyrusUndressingCallbackFunctor : public RE::BSScript::IStackCallbackFunctor {
            virtual inline void operator()(RE::BSScript::Variable a_result) override {
                if (a_result.IsNoneObject()) {
                    logger::info("result is none");
                } else if (a_result.IsBool()) {
                    MCM::MCMTable::setPapyusUndressing(a_result.GetBool());
                } else {
                    logger::info("result is not a bool");
                }
            }

            virtual inline void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_object){};
        };
    };

    struct PackageStart {
    public:
        static RE::TESPackage* thunk(RE::ExtraDataList* pthis, RE::Actor* actor) {
            if (OStim::ThreadManager::GetSingleton()->findActor(actor)) {
                // I don't know if anything else of importance happens in the orig function, so we just call it to make sure
                PackageStartOrig(pthis, actor);
                return Graph::LookupTable::OStimScenePackage;
            }

            return func(pthis, actor);
        }

        static inline REL::Relocation<decltype(thunk)> func;

        static inline void Install() {
            REL::Relocation<std::uintptr_t> target{RELOCATION_ID(36404, 37398), REL::VariantOffset(0x47, 0x47, 0x47)};
            auto& trampoline = SKSE::GetTrampoline();
            SKSE::AllocTrampoline(14);

            func = trampoline.write_branch<5>(target.address(), thunk);
        }

    private:
        inline static RE::TESPackage* PackageStartOrig(RE::ExtraDataList* pthis, RE::Actor* actor) {
            using func_t = decltype(PackageStartOrig);
            REL::Relocation<func_t> func{RELOCATION_ID(11918, 12057)};
            return func(pthis, actor);
        }
    };
}