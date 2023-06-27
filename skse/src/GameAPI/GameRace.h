#pragma once

#include "GameRecord.h"

namespace GameAPI {
    struct GameRace : public GameRecord<RE::TESRace> {
    public:
        inline GameRace() {}
        inline GameRace(RE::TESRace* race) { form = race; }
    };
}