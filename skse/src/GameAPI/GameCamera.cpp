#include "GameCamera.h"

#include "GameUtil.h"

#include "GameLogic/GameTable.h"

#include "Core/ThreadManager.h"

namespace GameAPI {
    void GameCamera::shakeCamera(float strength, float duration, bool firstPersonOnly) {
        if (firstPersonOnly && !RE::PlayerCamera::GetSingleton()->IsInFirstPerson()) {
            return;
        }

        RE::NiPoint3 location = RE::PlayerCharacter::GetSingleton()->GetPosition();
        ShakeCamera(strength, &location, duration);
    }


    void GameCamera::startSceneMode(bool freeCam) {
        if (GameLogic::GameTable::improvedCamSupport()) {
            RE::PlayerControls::GetSingleton()->data.povScriptMode = true;
        }

        RE::PlayerCamera* camera = RE::PlayerCamera::GetSingleton();
        if (freeCam) {
            if (!camera->IsInFreeCameraMode()) {
                camera->ForceThirdPerson();
                // the player moves to the scene location near instant, but not instant
                // so the free cam toggle has to be slightly delayed or it will not be at the scene location
                std::thread camThread = std::thread([&] {
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    SKSE::GetTaskInterface()->AddTask([] {
                        toggleFlyCamInner();
                    });
                });
                camThread.detach();
            }
        } else {
            if (GameLogic::GameTable::improvedCamSupport()) {
                camera->ForceFirstPerson();
                if (REL::Module::get().version().patch() < 1130) {
                    RE::ControlMap::GetSingleton()->enabledControls.reset(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
                } else {
                    // bandaid fix until CLib-NG is updated
                    auto ptr = &RE::ControlMap::GetSingleton()->enabledControls;
                    ptr += 8;
                    (*ptr).reset(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
                }
                camera->ForceThirdPerson();
            }
        }
    }

    void GameCamera::endSceneMode(bool firstPerson) {
        RE::PlayerCamera* camera = RE::PlayerCamera::GetSingleton();
        if (camera->IsInFreeCameraMode()) {
            SKSE::GetTaskInterface()->AddTask([] {
                toggleFlyCamInner();
            });
        } else if (GameLogic::GameTable::improvedCamSupport()) {
            if (REL::Module::get().version().patch() < 1130) {
                RE::ControlMap::GetSingleton()->enabledControls.set(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
            } else {
                // bandaid fix until CLib-NG is updated
                auto ptr = &RE::ControlMap::GetSingleton()->enabledControls;
                ptr += 8;
                (*ptr).set(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
            }
        }
        if (firstPerson) {
            SKSE::GetTaskInterface()->AddTask([] {
                RE::PlayerCamera::GetSingleton()->ForceFirstPerson();
            });
        }

        if (GameLogic::GameTable::improvedCamSupport()) {
            RE::PlayerControls::GetSingleton()->data.povScriptMode = false;
        }
    }

    void GameCamera::toggleFreeCam() {
        auto camera = RE::PlayerCamera::GetSingleton();

        if (!GameLogic::GameTable::improvedCamSupport()) {
            if (camera->IsInFirstPerson()) {
                camera->ForceThirdPerson();
            }

            SKSE::GetTaskInterface()->AddTask([] {
                toggleFlyCamInner();
            });
            return;
        }

        if (camera->IsInFreeCameraMode()) {
            SKSE::GetTaskInterface()->AddTask([] {
                toggleFlyCamInner();
                auto camera = RE::PlayerCamera::GetSingleton();
                camera->ForceFirstPerson();
                if (REL::Module::get().version().patch() < 1130) {
                    RE::ControlMap::GetSingleton()->enabledControls.reset(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
                } else {
                    // bandaid fix until CLib-NG is updated
                    auto ptr = &RE::ControlMap::GetSingleton()->enabledControls;
                    ptr += 8;
                    (*ptr).reset(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
                }
                camera->ForceThirdPerson();
            });
        } else {
            if (REL::Module::get().version().patch() < 1130) {
                RE::ControlMap::GetSingleton()->enabledControls.set(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
            } else {
                // bandaid fix until CLib-NG is updated
                auto ptr = &RE::ControlMap::GetSingleton()->enabledControls;
                ptr += 8;
                (*ptr).set(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
            }
            SKSE::GetTaskInterface()->AddTask([] {
                toggleFlyCamInner();
            });
        }
    }

    void GameCamera::toggleFlyCamInner() {
        const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
        const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
        if (script) {
            script->SetCommand("tfc");
            GameUtil::CompileAndRun(script, RE::PlayerCharacter::GetSingleton());
            delete script;
        }
    }


    void GameCamera::fadeToBlack(float fadeDuration) {
        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto args = RE::MakeFunctionArguments(std::move(fadeDuration));
            vm->DispatchStaticCall("OSKSE", "FadeToBlack", args, callback);
        }
    }

    void GameCamera::fadeFromBlack(float fadeDuration) {
        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto args = RE::MakeFunctionArguments(std::move(fadeDuration));
            vm->DispatchStaticCall("OSKSE", "FadeFromBlack", args, callback);
        }
    }
}