#pragma once

namespace GameAPI {
    class GameCamera {
    public:
        static void shakeCamera(float strength, float duration, bool firstPersonOnly);
        inline static void shakeCamera(float strength, float duration) { shakeCamera(strength, duration, false); }

        inline static float getWorldFOV() { return RE::PlayerCamera::GetSingleton()->worldFOV; }
        inline static void setWorldFOV(int fov) { RE::PlayerCamera::GetSingleton()->worldFOV = fov; }

        static void startSceneMode(bool freeCam);
        static void endSceneMode(bool firstPerson);
        static void toggleFreeCam();

    private:
        inline static void ShakeCamera(float strength, RE::NiPoint3* location, float duration) {
            using func_t = decltype(ShakeCamera);
            REL::Relocation<func_t> func{RELOCATION_ID(32275, 33012)};
            func(strength, location, duration);
        }

        static void toggleFlyCamInner();
    };
}