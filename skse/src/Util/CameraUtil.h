#pragma once

namespace CameraUtil {
    void toggleFlyCam();
    void toggleFlyCamInner();

    void shakeCamera(float strength, float duration, bool firstPersonOnly);
    void shakeCamera(float strength, float duration);

    class EnableFlyCamCallbackFunctor : public RE::BSScript::IStackCallbackFunctor {
        virtual inline void operator()(RE::BSScript::Variable a_result) override {
            toggleFlyCamInner();
        }

        virtual inline void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_object){};
    };
}