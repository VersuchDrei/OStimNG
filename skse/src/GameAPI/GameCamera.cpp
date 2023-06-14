#include "GameCamera.h"

#include "GameTable.h"

namespace GameAPI {
    void GameCamera::shakeCamera(float strength, float duration, bool firstPersonOnly) {
        if (firstPersonOnly && !RE::PlayerCamera::GetSingleton()->IsInFirstPerson()) {
            return;
        }

        RE::NiPoint3 location = RE::PlayerCharacter::GetSingleton()->GetPosition();
        ShakeCamera(strength, &location, duration);
    }


    void GameCamera::startSceneMode(bool freeCam) {
        RE::PlayerCamera* camera = RE::PlayerCamera::GetSingleton();
        if (freeCam) {
            if (!camera->IsInFreeCameraMode()) {
                camera->ForceThirdPerson();
                // the player moves to the scene location near instant, but not instant
                // so the free cam toggle has to be slightly delayed or it will not be at the scene location
                std::thread camThread = std::thread([&] {
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    toggleFlyCamInner();
                });
                camThread.detach();
            }
        } else {
            if (GameTable::improvedCamSupport()) {
                camera->ForceFirstPerson();
                RE::ControlMap::GetSingleton()->enabledControls.reset(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
                camera->ForceThirdPerson();
            }
        }
    }

    void GameCamera::endSceneMode(bool firstPerson) {
        RE::PlayerCamera* camera = RE::PlayerCamera::GetSingleton();
        if (camera->IsInFreeCameraMode()) {
            toggleFlyCamInner();
        } else if (GameTable::improvedCamSupport()) {
            auto controlMap = RE::ControlMap::GetSingleton();
            controlMap->enabledControls.set(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
        }
        if (firstPerson) {
            std::thread camThread = std::thread([&] {
                RE::PlayerCamera::GetSingleton()->ForceFirstPerson();
            });
            camThread.detach();
        }
    }

    void GameCamera::toggleFreeCam() {
        if (!GameTable::improvedCamSupport()) {
            toggleFlyCamInner();
            return;
        }

        auto camera = RE::PlayerCamera::GetSingleton();
        if (camera->IsInFreeCameraMode()) {
            toggleFlyCamInner();
            camera->ForceFirstPerson();
            RE::ControlMap::GetSingleton()->enabledControls.reset(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
            camera->ForceThirdPerson();
        } else {
            RE::ControlMap::GetSingleton()->enabledControls.set(RE::UserEvents::USER_EVENT_FLAG::kPOVSwitch);
            toggleFlyCamInner();
        }
    }

    void GameCamera::toggleFlyCamInner() {
        const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
        const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
        if (script) {
            const auto selectedRef = RE::Console::GetSelectedRef();
            script->SetCommand("tfc");
            script->CompileAndRun(selectedRef.get());
            delete script;
        }
    }
}