#include "Core/ThreadActor.h"

#include "Core/Thread.h"

#include "GameAPI/Game.h"
#include "Util/CameraUtil.h"

namespace OStim {
    void ThreadActor::orgasm(bool ignoreStall) {
        excitement = 100;

        if (!ignoreStall && (stallClimax || thread->getStallClimax())) {
            return;
        }

        if (thread->autoTransition(index, "climax")) {
            awaitingClimax = true;
        } else {
            climax();
        }
    }

    void ThreadActor::climax() {
        excitement = -3;

        awaitingClimax = false;
        timesClimaxed++;

        if (!muted && voiceSet && voiceSet->climax) {
            playEventExpression(voiceSet->climaxExpression);
            voiceSet->climax.play(actor, MCM::MCMTable::getMoanVolume());
        }

        if (thread->isPlayerThread()) {
            // TODO properly use GameActor here
            FormUtil::sendModEvent(actor.form, "ostim_orgasm", thread->getCurrentNode()->scene_id, index);

            if (isPlayer) {
                if (MCM::MCMTable::getBlurOnOrgasm()) {
                    FormUtil::apply(Util::LookupTable::OStimNutEffect, 1.0);
                }
            }

            if (MCM::MCMTable::getSlowMotionOnOrgasm()) {
                std::thread slowMoThread = std::thread([&]() {
                    GameAPI::Game::setGameSpeed(0.3);
                    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
                    GameAPI::Game::setGameSpeed(1);
                });
                slowMoThread.detach();
            }

            CameraUtil::shakeCamera(1.0, 2.0, true);

            if (MCM::MCMTable::useRumble()) {
                GameAPI::Game::shakeController(0.5, 0.5, 0.7);
            }
        }
        // TODO properly use GameActor here
        FormUtil::sendModEvent(actor.form, "ostim_actor_orgasm", thread->getCurrentNode()->scene_id, thread->m_threadId);

        actor.damageActorValue(GameAPI::GameActorValues::STAMINA, 250);

        // todo give other actor excitement when in vaginalsex

        if (schlong) {
            // TODO enable for player when we have our own UI
            if (!thread->isPlayerThread()) {
                thread->SetSpeed(0);
            }
        }

        if (MCM::MCMTable::endOnAllOrgasm()) {
            bool end = true;
            for (auto& [index, threadActor] : thread->getActors()) {
                if (threadActor.timesClimaxed == 0) {
                    end = false;
                    break;
                }
            }
            if (end) {
                thread->setStopTimer(4000);
            }
        } else {
            if (isPlayer && MCM::MCMTable::endOnPlayerOrgasm()) {
                thread->setStopTimer(4000);
            }
            if ((!female || schlong && MCM::MCMTable::futaUseMaleClimax()) ? MCM::MCMTable::endOnMaleOrgasm()
                                                                           : MCM::MCMTable::endOnFemaleOrgasm()) {
                thread->setStopTimer(4000);
            }
        }
        if (!thread->isPlayerThread() && MCM::MCMTable::endNPCSceneOnOrgasm()) {
            thread->setStopTimer(4000);
        }

        thread->setAutoModeToMainStage();
    }
}