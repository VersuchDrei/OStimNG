#pragma once

#include "PluginInterface/Graph/ActionTag.h"

namespace Graph {
    namespace Action {
        struct ActionTag : public OStim::ActionTag {
            std::string tag;

            virtual const char* getTagID() override;
        };
    }
}