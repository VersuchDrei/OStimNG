#include "CameraUtil.h"

#include "MCMTable.h"

namespace CameraUtil {
    void toggleFlyCam() {
        if (!MCM::MCMTable::supportImprovedCam()) {
            toggleFlyCamInner();
            return;
        }

        // this is probably the dirtiest shit in this entire dll
        // but I don't know if imporved cam has a messaging interface, so here we go anyways
        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            if (RE::PlayerCamera::GetSingleton()->IsInFreeCameraMode()) {
                toggleFlyCamInner();
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                auto args = RE::MakeFunctionArguments(std::move(false));
                vm->DispatchStaticCall("OSKSE", "ToggleImprovedCamera", args, callback);
            } else {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new EnableFlyCamCallbackFunctor());
                auto args = RE::MakeFunctionArguments(std::move(true));
                vm->DispatchStaticCall("OSKSE", "ToggleImprovedCamera", args, callback);
            }
        }
    }

    void toggleFlyCamInner() {
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