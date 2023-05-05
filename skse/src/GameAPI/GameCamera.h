#pragma once

namespace GameAPI {
    class GameCamera {
    public:
        static void shakeCamera(float strength, float duration, bool firstPersonOnly);

        inline static void shakeCamera(float strength, float duration) { shakeCamera(strength, duration, false); }

    private:
        inline static void ShakeCamera(float strength, RE::NiPoint3* location, float duration) {
            using func_t = decltype(ShakeCamera);
            REL::Relocation<func_t> func{RELOCATION_ID(32275, 33012)};
            func(strength, location, duration);
        }
    };
}