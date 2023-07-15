#pragma once

#include "GamePointer.h"

namespace GameAPI {
    struct GameSerializationInterface : public GamePointer<SKSE::SerializationInterface> {
    public:
        inline GameSerializationInterface(SKSE::SerializationInterface* serial) {
            object = serial;
        }
    };
}