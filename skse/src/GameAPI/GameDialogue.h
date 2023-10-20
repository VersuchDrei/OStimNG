#pragma once

#include "GameRecord.h"

namespace GameAPI {
    struct GameDialogue : public GameRecord<RE::TESTopic> {
        inline GameDialogue() {}
        inline GameDialogue(RE::TESTopic* topic) { form = topic; }
    };
}