#include "Core/ThreadActor.h"

#include "Core/Thread.h"

#include "GameAPI/Game.h"
#include "Util/APITable.h"
#include "Util/CameraUtil.h"

namespace OStim {
    void ThreadActor::loopClimax() {
        if (awaitingOrgasm) {
            if (!stallClimax && !thread->getStallClimax()) {
                orgasm(false);
            }
        } else if (awaitingClimaxInner) {
            if (!lastMoan && !actor.isTalking()) {
                climaxInner();
            }
        }
    }

    void ThreadActor::orgasm(bool ignoreStall) {
        if (awaitingOrgasm || awaitingClimax) {
            return;
        }

        excitement = 100;

        if (!ignoreStall && (stallClimax || thread->getStallClimax())) {
            awaitingOrgasm = true;
            setTimeUntilClimax(0.0f);
            return;
        }

        awaitingOrgasm = false;

        if (thread->autoTransition(index, "climax")) {
            awaitingClimax = true;
            setTimeUntilClimax(0.0f);
        } else {
            climax();
        }
    }

    void ThreadActor::climax() {
        awaitingClimax = false;
        setTimeUntilClimax(0.0f);

        if (lastMoan || actor.isTalking()) {
            awaitingClimaxInner = true;
        } else {
            climaxInner();
        }
    }

    void ThreadActor::climaxInner() {
        awaitingClimaxInner = false;
        excitement = -3;

        timesClimaxed++;
        actor.setFactionRank(Util::APITable::getTimesClimaxedFaction(), timesClimaxed);

        if (schlong) {
            thread->SetSpeed(0);
        }
        actor.damageActorValue(GameAPI::GameActorValues::STAMINA, 250);

        playClimaxSound();

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

        // todo give other actor excitement when in vaginalsex

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
            if ((!female || schlong && MCM::MCMTable::futaUseMaleClimax()) ? MCM::MCMTable::endOnMaleOrgasm() : MCM::MCMTable::endOnFemaleOrgasm()) {
                thread->setStopTimer(4000);
            }
        }
        if (!thread->isPlayerThread() && MCM::MCMTable::endNPCSceneOnOrgasm()) {
            thread->setStopTimer(4000);
        }

        thread->setAutoModeToMainStage();
    }

    void ThreadActor::setTimeUntilClimax(float time) {
        timeUntilClimax = time;
        if (timeUntilClimax < 0) {
            actor.setFactionRank(Util::APITable::getTimeUntilClimaxFaction(), -1);
        } else if (timeUntilClimax > 100.0f){
            actor.setFactionRank(Util::APITable::getTimeUntilClimaxFaction(), 101);
        } else {
            actor.setFactionRank(Util::APITable::getTimeUntilClimaxFaction(), static_cast<int>(timeUntilClimax));
        }
    }
}