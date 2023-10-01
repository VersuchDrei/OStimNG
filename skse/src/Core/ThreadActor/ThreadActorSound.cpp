#include "Core/ThreadActor.h"

namespace OStim {
    void ThreadActor::loopSound() {
        if (actor.isTalking()) {
            return;
        }

        if (soundGracePeriod > 0) {
            soundGracePeriod -= Constants::LOOP_TIME_MILLISECONDS;
            return;
        }

        if (lastMoan) {
            if (!lastMoan->isPlaying()) {
                lastMoan = nullptr;
                clearEventExpression();
            }
        } else if (moanCooldown > 0) {
            moanCooldown -= Constants::LOOP_TIME_MILLISECONDS;
            if (moanCooldown <= 0) {
                moan();
            }
        }
    }

    void ThreadActor::changeNodeSound() {
        if (graphActor->moan) {
            startMoanCooldown();
        } else {
            stopMoanCooldown();
        }
    }

    void ThreadActor::mute() {
        if (muted) {
            return;
        }

        stopMoanCooldown();
        muted = true;
    }

    void ThreadActor::unmute() {
        muted = false;
        startMoanCooldown();
    }

    void ThreadActor::startMoanCooldown() {
        if (muted || awaitingClimax || eventExpression || !graphActor->moan) {
            moanCooldown = -1;
            return;
        }

        bool doMoan = false;
        if (muffled || graphActor->muffled) {
            doMoan = !voiceSet.moanMuffled.empty();
        } else {
            doMoan = !voiceSet.moan.empty();
        }

        if (doMoan) {
            moanCooldown = std::uniform_int_distribution<>(MCM::MCMTable::getMoanIntervalMin(), MCM::MCMTable::getMoanIntervalMax())(Constants::RNG);
        } else {
            moanCooldown = -1;
        }
    }

    void ThreadActor::stopMoanCooldown() {
        moanCooldown = -1;
    }

    void ThreadActor::moan() {
        Sound::SoundSet* set = nullptr;
        if (muffled || graphActor->muffled) {
            set = voiceSet.getMoanMuffled(actor, primaryPartner);
        } else {
            set = voiceSet.getMoan(actor, primaryPartner);
        }

        if (set) {
            setEventExpression(set->expression);
            set->sound.play(actor, MCM::MCMTable::getMoanVolume());
            lastMoan = &set->sound;
            soundGracePeriod = 150;
        }
    }

    void ThreadActor::climaxMoan() {
        if (muted) {
            return;
        }

        Sound::SoundSet* set = nullptr;
        if (muffled || graphActor->muffled) {
            set = voiceSet.getClimaxMuffled(actor, primaryPartner);
        } else {
            set = voiceSet.getClimax(actor, primaryPartner);
        }

        if (set) {
            setEventExpression(set->expression);
            set->sound.play(actor, MCM::MCMTable::getMoanVolume());
            lastMoan = &set->sound;
            soundGracePeriod = 150;
        }
    }
}