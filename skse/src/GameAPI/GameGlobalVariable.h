#pragma once

#include "GameRecord.h"

namespace GameAPI {
    struct GameGlobalVariable : public GameRecord<RE::TESGlobal>{
    public:
        inline GameGlobalVariable() {}
        inline GameGlobalVariable(RE::TESGlobal* global) { form = global; }

        inline float getValue() {
            return form->value;
        }

        inline void setValue(float value) {
            form->value = value;
        }
    };
}