#pragma once

#include "GameRecord.h"

namespace GameAPI {
    struct GameFaction : public GameRecord<RE::TESFaction> {
    public:
        inline GameFaction() {}
        inline GameFaction(RE::TESFaction* faction) { form = faction; }
    };
}