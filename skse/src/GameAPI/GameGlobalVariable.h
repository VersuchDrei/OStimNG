#pragma once

#include "GameRecord.h"

namespace GameAPI {
    struct GameGlobalVariable : public GameRecord<RE::TESGlobal>{
    public:
        inline static GameGlobalVariable fromFile(std::string mod, uint32_t formID) {
            return RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(formID, mod);
        }

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