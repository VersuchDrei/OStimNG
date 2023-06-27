#pragma once

namespace GameAPI {
    struct GameGlobalVariable {
    public:
        inline static GameGlobalVariable fromFile(std::string mod, uint32_t formID) {
            return RE::TESDataHandler::GetSingleton()->LookupForm<RE::TESGlobal>(formID, mod);
        }

        RE::TESGlobal* global;

        inline GameGlobalVariable(RE::TESGlobal* global) : global{global} {}

        inline operator bool() const { return global; }
        inline bool operator==(const GameGlobalVariable other) { return global == other.global; }
        inline bool operator!=(const GameGlobalVariable other) { return global != other.global; }

        inline float getValue() {
            return global->value;
        }

        inline void setValue(float value) {
            global->value = value;
        }
    };
}