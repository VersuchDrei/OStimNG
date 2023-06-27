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

    void Game::showMessageBox(std::string content, std::vector<std::string> options, std::function<void(unsigned int)> callback) {
        auto* messagebox = RE::MessageDataFactoryManager::GetSingleton()->GetCreator<RE::MessageBoxData>(RE::InterfaceStrings::GetSingleton()->messageBoxData)->Create();
        messagebox->callback = RE::make_smart<MessageBoxCallback>(callback);
        messagebox->bodyText = content;
        for (std::string& text : options) {
            messagebox->buttonText.push_back(text.c_str());
        }
        messagebox->QueueMessage();
    }
}