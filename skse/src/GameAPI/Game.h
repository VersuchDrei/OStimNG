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

        inline static int getMessageBoxOptionLimit() { return 9; }
        static void showMessageBox(std::string content, std::vector<std::string> options, std::function<void(unsigned int)> callback);

    private:
        class MessageBoxCallback : public RE::IMessageBoxCallback {
        public:
            MessageBoxCallback(std::function<void(unsigned int)> callback) : callback{callback} {}
            ~MessageBoxCallback() override {}
            void Run(RE::IMessageBoxCallback::Message message) override { callback(static_cast<unsigned int>(message)); }
        private:
            std::function<void(unsigned int)> callback;
        };

        inline static void ShakeController(bool leftMotor, float strength, float duration) {
            using func_t = decltype(ShakeController);
            REL::Relocation<func_t> func{RELOCATION_ID(67220, 68528)};
            func(leftMotor, strength, duration);
        }
    };
}