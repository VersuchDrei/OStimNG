#include "EventListener.h"

#include "Core/ThreadManager.h"
#include "Serial/Manager.h"
#include "UI/Align/AlignMenu.h"
#include "UI/UIState.h"
#include "Util/CameraUtil.h"
#include "MCM/MCMTable.h"
#include "Util.h"

namespace Events {
    RE::BSEventNotifyControl EventListener::ProcessEvent(const RE::TESLoadGameEvent* a_event, RE::BSTEventSource<RE::TESLoadGameEvent>* a_eventSource) {
        logger::info("game loaded");

        Serialization::closeOldThreads();

        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new CheckPapyrusUndressingCallbackFunctor());
            auto args = RE::MakeFunctionArguments();
            vm->DispatchStaticCall("OUndress", "UsePapyrusUndressing", args, callback);
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl EventListener::ProcessEvent(const SKSE::NiNodeUpdateEvent* a_event, RE::BSTEventSource<SKSE::NiNodeUpdateEvent>* a_eventSource) {
        if (a_event->reference->Is(RE::Actor::FORMTYPE)) {
            RE::Actor* actor = a_event->reference->As<RE::Actor>();
            OStim::ThreadActor* threadActor = OStim::ThreadManager::GetSingleton()->findActor(actor);
            if (threadActor) {
                threadActor->handleNiNodeUpdate();
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl EventListener::ProcessEvent(RE::InputEvent* const* a_events, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) {
        if (!a_events) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (RE::UI::GetSingleton()->GameIsPaused()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!OStim::ThreadManager::GetSingleton()->playerThreadRunning()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        for (RE::InputEvent* iEvent = *a_events; iEvent; iEvent = iEvent->next) {
            if (iEvent->GetEventType() != RE::INPUT_EVENT_TYPE::kButton) {
                continue;
            }

            RE::ButtonEvent* bEvent = iEvent->AsButtonEvent();

            uint32_t keyCode;
            RE::INPUT_DEVICE deviceType = bEvent->GetDevice();
            uint32_t keyMask = bEvent->idCode;

            if (deviceType == RE::INPUT_DEVICE::kMouse) {
                keyCode = 256 + keyMask;
            } else if (deviceType == RE::INPUT_DEVICE::kGamepad) {
                keyCode = util::gamepadMaskToKeyCode(keyMask);
            } else {
                keyCode = keyMask;
            }

            if (keyCode >= 282) {
                // invalid key
                continue;
            }

            if (bEvent->IsDown()) {
                if (keyCode == MCM::MCMTable::keyAlignment()) {
                    auto uiState = UI::UIState::GetSingleton();
                    logger::info("align key fired");
                    uiState->ToggleActiveMenu(UI::kAlignMenu);
                } else if (keyCode == MCM::MCMTable::keyFreeCam()) {
                    CameraUtil::toggleFlyCam();
                }
                else if (keyCode == 37) {
                    auto uiState = UI::UIState::GetSingleton();
                    uiState->ToggleActiveMenu(UI::kSearchMenu);
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }
}