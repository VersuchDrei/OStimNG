#include "Core/ThreadActor.h"

#include "Core/Thread.h"

#include "Util/Constants.h"
#include "Util/RNGUtil.h"

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
            return;
        }

        if (lastMoan) {
            if (!lastMoan->isPlaying()) {
                lastMoan = nullptr;
                clearEventExpression();
            }
            return;
        }

        if (eventTimer > 0) {
            eventTimer -= Constants::LOOP_TIME_MILLISECONDS;

            if (eventTimer <= 0) {
                playSound(eventReaction, eventPartner, true);
                eventReaction = nullptr;
                eventPartner = nullptr;
            }

            return;
        }

        if (awaitingClimax || awaitingClimaxInner) {
            return;
        }
        
        if (moanCooldown > 0) {
            moanCooldown -= Constants::LOOP_TIME_MILLISECONDS;
            if (moanCooldown <= 0) {
                playSound(&voiceSet.moan, primaryPartner, false);
            }
        }
    }

    void ThreadActor::mute() {
        if (muted) {
            return;
        }

        muted = true;
    }

    void ThreadActor::unmute() {
        muted = false;
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
        if (moanCooldown > 0) {
            return;
        }

        moanCooldown = RNGUtil::uniformInt(MCM::MCMTable::getMoanIntervalMin(), MCM::MCMTable::getMoanIntervalMax());
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

    void ThreadActor::playSound(Sound::ReactionSet* reactionSet, GameAPI::GameActor partner, bool ignoreChecks) {
        moanCooldown = 0;
        if ((ignoreChecks || graphActor->talk) && canTalk()) {
            dialogueCountdown--;
            Sound::DialogueSet* set = reactionSet->getDialogue(actor, partner);

            if (set) {
                if (dialogueCountdown <= set->dialogueThreshold) {
                    if (set->expression != "") {
                        setEventExpression(set->expression);
                    }
                    actor.sayTo(primaryPartner, set->dialogue);
                    isTalking = true;
                    soundGracePeriod = 250;
                    moanCooldown = set->moanIntervalOverride;
                    setDialogueCountdown();
                    startMoanCooldown();
                    return;
                }
            }
        }

        if ((ignoreChecks || graphActor->moan) && canMakeSound()) {
            Sound::SoundSet* set = nullptr;
            if (muffled || graphActor->muffled) {
                set = reactionSet->getSoundMuffled(actor, partner);
            } else {
                set = reactionSet->getSound(actor, partner);
            }

            if (set) {
                setEventExpression(set->expression);
                set->sound.play(actor, MCM::MCMTable::getMoanVolume());
                lastMoan = &set->sound;
                soundGracePeriod = 150;
                moanCooldown = set->moanIntervalOverride;
            }
        }

        startMoanCooldown();
    }


    void ThreadActor::setDialogueCountdown() {
        if (female) {
            dialogueCountdown = RNGUtil::uniformInt(MCM::MCMTable::getFemaleDialogueCountdownMin(), MCM::MCMTable::getFemaleDialogueCountdownMax()) + 1;
        } else {
            dialogueCountdown = RNGUtil::uniformInt(MCM::MCMTable::getMaleDialogueCountdownMin(), MCM::MCMTable::getMaleDialogueCountdownMax()) + 1;
        }
    }

    void ThreadActor::playClimaxSound() {
        for (auto& [index, threadActor] : thread->getActors()) {
            if (threadActor.actor != actor) {
                threadActor.reactToClimax(actor);
            }
        }

        dialogueCountdown = 0;
        playSound(&voiceSet.climax, primaryPartner, true);

        // post climax comment
        // the event timer will only start counting down once the previous sound has stopped playing
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
}