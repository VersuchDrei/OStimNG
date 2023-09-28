#include "Core/ThreadActor.h"

namespace OStim {
    void ThreadActor::loopSound() {
        if (actor.isTalking()) {
            return;
        }

        if (lastMoan) {
            if (!lastMoan->isPlaying()) {
                logger::info("clearing moan expression");
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
            set = voiceSet.getMoanMuffled(actor);
        } else {
            set = voiceSet.getMoan(actor);
        }

        if (set) {
            logger::info("starting moaning");
            setEventExpression(set->expression);
            set->sound.play(actor, MCM::MCMTable::getMoanVolume());
            lastMoan = &set->sound;
        }
    }

    void ThreadActor::climaxMoan() {
        if (muted) {
            return;
        }

        Sound::SoundSet* set = nullptr;
        if (muffled || graphActor->muffled) {
            set = voiceSet.getClimaxMuffled(actor);
        } else {
            set = voiceSet.getClimax(actor);
        }

        if (set) {
            setEventExpression(set->expression);
            set->sound.play(actor, MCM::MCMTable::getMoanVolume());
            lastMoan = &set->sound;
        }
    }
}