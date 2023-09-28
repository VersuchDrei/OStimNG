#include "APITable.h"

namespace Util {
    void APITable::setupForms() {
        OStimExcitementFaction.loadFile("OStim.esp", 0xD93);
        OStimTimesClimaxedFaction.loadFile("OStim.esp", 0xE49);
        OStimTimeUntilClimaxFaction.loadFile("OStim.esp", 0xE4A);
    }
}