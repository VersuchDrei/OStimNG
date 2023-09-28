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

        inline SoundSet* getMoan(GameAPI::GameActor actor) { return getSound(moan, actor); }
        inline SoundSet* getMoanMuffled(GameAPI::GameActor actor) { return getSound(moanMuffled, actor); }
        inline SoundSet* getClimax(GameAPI::GameActor actor) { return getSound(climax, actor); }
        inline SoundSet* getClimaxMuffled(GameAPI::GameActor actor) { return getSound(climaxMuffled, actor); }

    private:
        static SoundSet* getSound(std::vector<SoundSet>& sounds, GameAPI::GameActor actor);
    };
}