#pragma once

#include "ToyWrapper.h"

#include "Core/ThreadActor.h"

namespace Toys {
    class ToyHandler {
    public:
        ToyHandler(OStim::ThreadActor* actor, std::string slot, ToyWrapper* toy);

        void loop();
        void update(uint16_t peakInterval);
        void stop();

    private:
        OStim::ThreadActor* actor;
        Settings::SlotSettings* settings;
        ToyWrapper* toy;
        uint16_t updateInterval;
        int16_t updateCooldown = 0;
    };
}