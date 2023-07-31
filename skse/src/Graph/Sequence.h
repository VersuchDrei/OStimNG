#pragma once

#include "SequenceEntry.h"

#include "Trait/Condition.h"

namespace Graph {
    struct Sequence {
        std::string id;
        std::vector<SequenceEntry> nodes;
        std::vector<std::string> tags;

        bool fulfilledBy(std::vector<Trait::ActorCondition> conditions);
    };
}