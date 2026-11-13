#pragma once

#include "NodeActorTag.h"
#include "NodeActorTagVisitor.h"
#include "GenitalAlignment.h"

namespace OStim {
    class NodeActor {
    public:
        // --- ABI version 1 ---
        virtual bool hasTag(const char* tag) = 0;
        virtual uint32_t getTagCount() = 0;
        virtual NodeActorTag* getTag(uint32_t index) = 0;
        virtual void forEachTag(NodeActorTagVisitor* visitor) = 0;

        // --- ABI version 0x07050021 ---
        virtual GenitalAlignment getGenitalAlignment() = 0;
        virtual uint32_t getGenitalAlignmentTarget() = 0;
    };
}