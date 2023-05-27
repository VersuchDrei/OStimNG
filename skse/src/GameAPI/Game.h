#pragma once

#include "GameAPI/GameActor.h"
#include "GameAPI/GameTable.h"

namespace GameAPI {
    class Game {
    public:
        inline static float getTimeScale() { return GameTable::getTimescale()->value; }
        inline static void setTimeScale(float scale) { GameTable::getTimescale()->value = scale; }
        static void setGameSpeed(float speed);
        static void shakeController(float leftStrength, float rightStrength, float duration);
        static GameActor getCrosshairActor();

    private:
        inline static void ShakeController(bool leftMotor, float strength, float duration) {
            using func_t = decltype(ShakeController);
            REL::Relocation<func_t> func{RELOCATION_ID(67220, 68528)};
            func(leftMotor, strength, duration);
        }
    };
}