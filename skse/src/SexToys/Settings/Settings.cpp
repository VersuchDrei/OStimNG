#include "Settings.h"

namespace Toys {
    namespace Settings {
        ToySettings* Settings::getToySettings(std::string id) {
            return &toySettings.insert({id, {}}).first->second;
        }
    }
}