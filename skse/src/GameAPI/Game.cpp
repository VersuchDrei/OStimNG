#include "Game.h"

namespace GameAPI {
    void Game::setGameSpeed(float speed) {
        const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
        const auto script = scriptFactory ? scriptFactory->Create() : nullptr;
        if (script) {
            const auto selectedRef = RE::Console::GetSelectedRef();
            script->SetCommand("sgtm " + std::to_string(speed));
            script->CompileAndRun(selectedRef.get());
            delete script;
        }
    }

    void Game::shakeController(float leftStrength, float rightStrength, float duration) {
        ShakeController(true, leftStrength, duration);
        ShakeController(true, rightStrength, duration);
    }

    GameActor Game::getCrosshairActor() {
        if (GameTable::getCurrentCrosshairRef()) {
            return GameTable::getCurrentCrosshairRef()->As<RE::Actor>();
        } else {
            return {};
        }
    }
}