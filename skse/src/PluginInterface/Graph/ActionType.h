#pragma once

#include "ActionTag.h"

namespace OStim {
    class ActionType {
    public:
        const char* getActionID() = 0;

        virtual bool hasTag(const char* tag) = 0;
        virtual uint32_t getTagCount() = 0;
        virtual ActionTag* getTag(uint32_t index) = 0;
    };
}