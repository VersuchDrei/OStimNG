#include "Core/ThreadActor.h"

#include "Core/Thread.h"

namespace OStim {
    void ThreadActor::loopSound() {
        if (soundGracePeriod > 0) {
            soundGracePeriod -= Constants::LOOP_TIME_MILLISECONDS;
            return;
        }

        if (actor.isTalking()) {
            return;
        } else if (isTalking) {
            isTalking = false;
            clearEventExpression();

            if (isPlayingClimaxSound) {
                reactToOwnClimax();
            }
            return;
        }

        if (eventTimer > 0) {
            eventTimer -= Constants::LOOP_TIME_MILLISECONDS;

            if (eventTimer <= 0) {
                playEventSound();
                eventReaction = nullptr;
                eventPartner = nullptr;
            }

            return;
        }

        if (lastMoan) {
            if (!lastMoan->isPlaying()) {
                lastMoan = nullptr;
                clearEventExpression();
                if (isPlayingClimaxSound) {
                    reactToOwnClimax();
                }
            }
        } else if (moanCooldown > 0) {
            moanCooldown -= Constants::LOOP_TIME_MILLISECONDS;
            if (moanCooldown <= 0) {
                dialogueCountdown--;
                if (dialogueCountdown <= 0) {
                    talk();
                } else {
                    moan();
                }
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


    bool ThreadActor::canMakeSound() {
        if (actor.isTalking()) {
            return false;
        }

        if (lastMoan) {
            return false;
        }

        return true;
    }

    bool ThreadActor::canTalk() {
        if (isPlayer) {
            return false;
        }

        if (actor.isTalking()) {
            return false;
        }

        if (lastMoan) {
            return false;
        }

        if (muffled || graphActor->muffled) {
            return false;
        }

        for (auto& [index, threadActor] : thread->getActors()) {
            if (threadActor.actor == actor) {
                continue;
            }

            if (threadActor.actor.getVoice() == actor.getVoice()) {
                if (threadActor.actor.isTalking()) {
                    return false;
                }
            }
        }

        return true;
    }


    void ThreadActor::startMoanCooldown() {
        if (muted || awaitingClimax || eventExpression || !graphActor->moan) {
            moanCooldown = -1;
            return;
        }

        bool doMoan = false;
        if (muffled || graphActor->muffled) {
            doMoan = !voiceSet.moan.soundMuffled.empty();
        } else {
            doMoan = !voiceSet.moan.sound.empty();
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
            set = voiceSet.moan.getSoundMuffled(actor, primaryPartner);
        } else {
            set = voiceSet.moan.getSound(actor, primaryPartner);
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
            set = voiceSet.climax.getSoundMuffled(actor, primaryPartner);
        } else {
            set = voiceSet.climax.getSound(actor, primaryPartner);
        }

        if (set) {
            setEventExpression(set->expression);
            set->sound.play(actor, MCM::MCMTable::getMoanVolume());
            lastMoan = &set->sound;
            soundGracePeriod = 150;
            isPlayingClimaxSound = true;
        }
    }

    void ThreadActor::eventMoan() {
        if (muted) {
            return;
        }

        Sound::SoundSet* set = nullptr;
        if (muffled || graphActor->muffled) {
            set = eventReaction->getSoundMuffled(actor, eventPartner);
        } else {
            set = eventReaction->getSound(actor, eventPartner);
        }

        if (set) {
            setEventExpression(set->expression);
            set->sound.play(actor, MCM::MCMTable::getMoanVolume());
            lastMoan = &set->sound;
            soundGracePeriod = 150;
        }
    }


    void ThreadActor::setDialogueCountdown() {
        if (female) {
            dialogueCountdown = std::uniform_int_distribution<>(MCM::MCMTable::getFemaleDialogueCountdownMin(), MCM::MCMTable::getFemaleDialogueCountdownMax())(Constants::RNG) + 1;
        } else {
            dialogueCountdown = std::uniform_int_distribution<>(MCM::MCMTable::getMaleDialogueCountdownMin(), MCM::MCMTable::getMaleDialogueCountdownMax())(Constants::RNG) + 1;
        }
    }

    void ThreadActor::talk() {
        if (!canTalk() || !graphActor->talk) {
            moan();
            return;
        }

        Sound::DialogueSet* set = voiceSet.moan.getDialogue(actor, primaryPartner);

        if (set) {
            if (set->expression != "") {
                setEventExpression(set->expression);
            }
            actor.sayTo(primaryPartner, set->dialogue);
            isTalking = true;
            soundGracePeriod = 250;
        } else {
            moan();
            return;
        }

        setDialogueCountdown();
    }

    void ThreadActor::playClimaxSound() {
        for (auto& [index, threadActor] : thread->getActors()) {
            if (threadActor.actor != actor) {
                threadActor.reactToClimax(actor);
            }
        }

        if (!canMakeSound()) {
            return;
        }

        if (!canTalk()) {
            climaxMoan();
            return;
        }

        Sound::DialogueSet* set = voiceSet.climax.getDialogue(actor, primaryPartner);

        if (set) {
            if (set->expression != "") {
                setEventExpression(set->expression);
            }
            actor.sayTo(primaryPartner, set->dialogue);
            isTalking = true;
            isPlayingClimaxSound = true;
            soundGracePeriod = 250;
        } else {
            climaxMoan();
            return;
        }

        setDialogueCountdown();
    }

    void ThreadActor::playEventSound() {
        if (!canMakeSound()) {
            return;
        }

        dialogueCountdown--;
        if (dialogueCountdown > 0 || !canTalk()) {
            eventMoan();
            return;
        }

        Sound::DialogueSet* set = eventReaction->getDialogue(actor, eventPartner);

        if (set) {
            if (set->expression != "") {
                setEventExpression(set->expression);
            }
            actor.sayTo(eventPartner, set->dialogue);
            isTalking = true;
            soundGracePeriod = 250;
        } else {
            eventMoan();
            return;
        }

        setDialogueCountdown();
    }

    void ThreadActor::reactToEvent(int timer, std::string type, GameAPI::GameActor partner, std::function<std::unordered_map<std::string, Sound::ReactionSet>*(Sound::VoiceSet&)> setGetter) {
        std::unordered_map<std::string, Sound::ReactionSet>* set = setGetter(voiceSet);

        auto iter = set->find(type);
        if (iter == set->end()) {
            return;
        }

        eventTimer = timer;
        eventReaction = &iter->second;
        eventPartner = partner;
    }

    void ThreadActor::reactToClimax(GameAPI::GameActor partner) {
        if (voiceSet.climaxCommentOther.empty()) {
            return;
        }

        if (!voiceSet.climaxCommentOther.dialogue.empty()) {
            dialogueCountdown = 0;
        }

        eventTimer = 750;
        eventReaction = &voiceSet.climaxCommentOther;
        eventPartner = partner;
    }

    void ThreadActor::reactToOwnClimax() {
        isPlayingClimaxSound = false;

        if (voiceSet.climaxCommentSelf.empty()) {
            return;
        }

        if (!voiceSet.climaxCommentSelf.dialogue.empty()) {
            dialogueCountdown = 0;
        }

        eventTimer = 1000;
        eventReaction = &voiceSet.climaxCommentSelf;
        eventPartner = actor;
    }
}