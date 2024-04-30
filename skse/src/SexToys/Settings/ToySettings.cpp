#include "ToySettings.h"

namespace Toys {
    namespace Settings {
        SlotSettings* ToySettings::getSlotSettings(std::string slot) {
            return &slotSettings.insert({slot, {}}).first->second;
        }
    }
}