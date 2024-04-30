#pragma once

#include "ToySettings.h"

namespace Toys {
    namespace Settings {
        struct Settings {
            SynchronizationType synchronizationType = SynchronizationType::PLAYER;
            std::unordered_map<std::string, ToySettings> toySettings;

            
            ToySettings* getToySettings(std::string id);
        };
    }
}