#pragma once

#include "GameAPI/GameSound.h"

namespace Sound {
    struct VoiceSet {
        GameAPI::GameSound moan;
        GameAPI::GameSound moanMuffled;
        std::string moanExpression = "moan";
        GameAPI::GameSound climax;
        GameAPI::GameSound climaxMuffled;
        std::string climaxExpression = "climax";
    };
}