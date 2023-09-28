#include "VoiceSet.h"

namespace Sound {
    SoundSet* VoiceSet::getSound(std::vector<SoundSet>& sounds, GameAPI::GameActor actor) {
        for (SoundSet& sound : sounds) {
            if (!sound.condition || sound.condition.fulfills(actor)) {
                return &sound;
            }
        }

        return nullptr;
    }
}