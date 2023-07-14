#pragma once

#include "GamePointer.h"

namespace GameAPI {
    struct GameSerializationInterface : public GamePointer<RE::SerializationInterface> {
    public:
        inline GameSerializationInterface(RE::SerializationInterface* serial) {
            object = serial;
        }
    };
}