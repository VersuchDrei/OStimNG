#pragma once

#include "ActionType.h"

namespace OStim {
    class Action {
    public:
        virtual ActionType* getType() = 0;

        virtual uint32_t getActor() = 0;
        virtual uint32_t getTarget() = 0;
        virtual uint32_t getPerformer() = 0;
    };
}