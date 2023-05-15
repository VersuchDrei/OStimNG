#include "GameSound.h"

namespace GameAPI {
    void GameSound::play(GameActor actor, float volume) {
        RE::BSSoundHandle handle;
        RE::BSAudioManager::GetSingleton()->BuildSoundDataFromDescriptor(handle, form, 0x10);
        handle.SetObjectToFollow(actor.actor->Get3D());
        handle.SetVolume(volume);
        handle.Play();
    }
}