#pragma once

#include "ToySettings.h"

namespace Toys {
    namespace Settings {
        struct Settings {
            SynchronizationType synchronizationType = SynchronizationType::PLAYER;
            std::unordered_map<std::string, ToySettings> toySettings;

            ToySettings* getToySettings(std::string id);

            void serialize(GameAPI::GameSerializationInterface serial);
            void deserialize(GameAPI::GameSerializationInterface serial);
            void reset();
            void writeJson(json& json);
            void loadJson(json& json);
        };
    }
}