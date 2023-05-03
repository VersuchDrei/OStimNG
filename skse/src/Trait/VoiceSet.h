#pragma once

namespace Trait {
    struct VoiceSet {
        RE::BGSSoundDescriptorForm* moan = nullptr;
        RE::BGSSoundDescriptorForm* moanMuffled = nullptr;
        std::string moanExpression = "moan";
        RE::BGSSoundDescriptorForm* climax = nullptr;
        RE::BGSSoundDescriptorForm* climaxMuffled = nullptr;
        std::string climaxExpression = "climax";
    };
}