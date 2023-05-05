#pragma once

namespace GameAPI {
    class Game {
    public:
        static void setGameSpeed(float speed);
        static void shakeController(float leftStrength, float rightStrength, float duration);

    private:
        inline static void ShakeController(bool leftMotor, float strength, float duration) {
            using func_t = decltype(ShakeController);
            REL::Relocation<func_t> func{RELOCATION_ID(67220, 68528)};
            func(leftMotor, strength, duration);
        }
    };
}