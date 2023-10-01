#pragma once

#include "SoundSet.h"

#include "GameAPI/GameSound.h"

namespace Sound {
    struct VoiceSet {
    public:
        std::vector<SoundSet> moan;
        std::vector<SoundSet> moanMuffled;
        std::vector<SoundSet> climax;
        std::vector<SoundSet> climaxMuffled;

        inline SoundSet* getMoan(GameAPI::GameActor actor, GameAPI::GameActor target) { return getSound(moan, actor, target); }
        inline SoundSet* getMoanMuffled(GameAPI::GameActor actor, GameAPI::GameActor target) { return getSound(moanMuffled, actor, target); }
        inline SoundSet* getClimax(GameAPI::GameActor actor, GameAPI::GameActor target) { return getSound(climax, actor, target); }
        inline SoundSet* getClimaxMuffled(GameAPI::GameActor actor, GameAPI::GameActor target) { return getSound(climaxMuffled, actor, target); }

    private:
        static SoundSet* getSound(std::vector<SoundSet>& sounds, GameAPI::GameActor actor, GameAPI::GameActor target);
    };
}