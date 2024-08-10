#pragma once

#include "NodeActorTag.h"

namespace OStim {
    class NodeActor {
    public:
        virtual bool hasTag(const char* tag) = 0;
        virtual uint32_t getTagCount() = 0;
        virtual NodeActorTag* getTag(uint32_t index) = 0;
    };
}