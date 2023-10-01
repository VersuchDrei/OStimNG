#include "VoiceSet.h"

namespace Sound {
    SoundSet* VoiceSet::getSound(std::vector<SoundSet>& sounds, GameAPI::GameActor actor, GameAPI::GameActor target) {
        for (SoundSet& sound : sounds) {
            if (!sound.condition || sound.condition.fulfills(actor, target)) {
                return &sound;
            }
        }

        return nullptr;
    }
}