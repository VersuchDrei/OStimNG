#include "Thread.h"

#include "ThreadManager.h"

#include "Furniture/FurnitureTable.h"
#include "GameAPI/Game.h"
#include "GameAPI/GameCamera.h"
#include "Graph/GraphTable.h"
#include "Graph/Node.h"
#include <Messaging/IMessages.h>
#include "UI/Align/AlignMenu.h"
#include "UI/Scene/SceneMenu.h"
#include "UI/UIState.h"
#include "Util/CameraUtil.h"
#include "Util/Constants.h"
#include "Util/ControlUtil.h"
#include "Util/FormUtil.h"
#include "Util/LookupTable.h"
#include "Util/MathUtil.h"
#include "MCM/MCMTable.h"
#include "Util/APITable.h"
#include "Util/LookupTable.h"
#include "Util/ObjectRefUtil.h"
#include "Util/StringUtil.h"
#include "Util.h"

namespace OStim {
    Thread::Thread(int threadID, ThreadStartParams params) : m_threadId{threadID}, furniture{params.furniture} {
        for (GameAPI::GameActor actor : params.actors) {
            playerThread |= actor.isPlayer();
        }

        threadFlags = params.threadFlags;

        // --- setting up the vehicle --- //
        this->center = furniture ? furniture.getPosition() : (playerThread ? GameAPI::GameActor::getPlayer().getPosition() : params.actors.front().getPosition());

        if (furniture) {
            furnitureType = Furniture::FurnitureTable::getFurnitureType(furniture, false);
            furnitureOwner = furniture.getOwner();
            furniture.disableUse();

            Furniture::FurnitureOffset offset = Furniture::getOffset(furniture);
            float angle = furniture.getRotation();
            float sin = std::sin(angle);
            float cos = std::cos(angle);
            center.r += offset.rotation;
            center.x +=  cos * offset.x + sin * offset.y;
            center.y += -sin * offset.x + cos * offset.y;
            center.z += offset.z;
            furnitureScaleMult = offset.scale;
        } else {
            furnitureType = Furniture::FurnitureTable::getFurnitureType("none");
        }

        for (int i = 0; i < params.actors.size(); i++) {
            // TODO GameActor
            addActorInner(i, params.actors[i].form);
        }

        for (auto& [index, actor] : m_actors) {
            Util::APITable::getActorCountFaction().add(actor.getActor(), getActorCount());
        }

        if (furniture && MCM::MCMTable::resetClutter()) {
            // TODO GameObject
            Furniture::resetClutter(furniture.form, MCM::MCMTable::resetClutterRadius() * 100);
        }

        if (playerThread) {
            GameAPI::GameCamera::startSceneMode(MCM::MCMTable::useFreeCam());

            RE::INISettingCollection* ini = RE::INISettingCollection::GetSingleton();
            RE::Setting* speed = ini->GetSetting("fFreeCameraTranslationSpeed:Camera");
            if (speed) {
                freeCamSpeedBefore = speed->data.f;
                speed->data.f = MCM::MCMTable::freeCamSpeed();
            }

            worldFOVbefore = GameAPI::GameCamera::getWorldFOV();
            GameAPI::GameCamera::setWorldFOV(MCM::MCMTable::freeCamFOV());

            timeScaleBefore = GameAPI::Game::getTimeScale();
            if (MCM::MCMTable::customTimeScale() > 0) {
                GameAPI::Game::setTimeScale(MCM::MCMTable::customTimeScale());
            }
        }

        if ((params.threadFlags & ThreadFlag::NO_AUTO_MODE) == 0) {
            evaluateAutoMode();
        }
        
        if (params.duration > 0) {
            stopTimer = params.duration;
        } else if (!playerThread) {
            stopTimer = MCM::MCMTable::npcSceneDuration();
        }

        endAfterSequence = params.endAfterSequence;
    }

    Thread::~Thread() {
        for (auto& actorIt : m_actors) {
            removeActorSink(actorIt.second.getActor().form);
        }

        for (auto& soundPlayer : soundPlayers) {
            delete soundPlayer;
        }

        if (playerThread) {
            UI::HideMenus();
        }
    }

    void Thread::initContinue() {
        // this stuff needs to happen after the thread has been put into the map in thread manager
        for (auto& [index, actor] : m_actors) {
            actor.getActor().updateAI();
        }

        if (playerThread) {
            auto uiState = UI::UIState::GetSingleton();
            if (uiState) {
                uiState->SetThread(this);
            }
            UI::Scene::SceneMenu::GetMenu()->Show();
        }

        if (!MCM::MCMTable::onlyLightInDark() || GetActor(0)->getActor().getLightLevel() < 20) {
            logger::info("trying to add facelights");
            for (auto& [index, actor] : m_actors) {
                if (!actor.getActor().hasLight()) {
                    actor.equipObject("light");
                }
            }
        }

        if (playerThread) {
            FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_prestart", "", 0);
            FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_start", "", 0);
        }
        FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_thread_start", "", m_threadId);
    }

    void Thread::rebuildAlignmentKey() {
        Alignment::ThreadKey key;

        for (int i = 0; i < m_actors.size(); i++) {
            key.keys.push_back(m_actors.find(i)->second.getAlignmentKey());
        }

        alignmentKey = key.toString();

        if (playerThread) {
            UI::UIState::GetSingleton()->NodeChanged(this, m_currentNode);
        }   
    }

    std::string Thread::getAlignmentKey() {
        return alignmentKey;
    }

    Alignment::ActorAlignment Thread::getActorAlignment(int index) {
        return Alignment::Alignments::getActorAlignment(alignmentKey, m_currentNode, index);
    }

    void Thread::Navigate(std::string sceneId) {
        for (auto& nav : m_currentNode->navigations) {
            if (sceneId == nav.nodes.front()->scene_id) {
                ChangeNode(nav.nodes.front());
            }
        }      
    }

    void Thread::ChangeNode(Graph::Node* a_node) {
        if (m_currentNode == a_node) {
            return;
        }

        logger::info("thread {} changed to node {}", m_threadId, a_node->scene_id);

        std::unique_lock<std::shared_mutex> writeLock(nodeLock);
        if (a_node->isTransition && nodeQueue.empty() && !a_node->navigations.empty()) {
            nodeQueueCooldown = a_node->animationLengthMs;
            for (Graph::Node* navNode : a_node->navigations[0].nodes) {
                nodeQueue.push({navNode->animationLengthMs, navNode});
            }
        }
        m_currentNode = a_node;

        for (auto& [position, actor] : m_actors) {
            // --- excitement calculation --- //
            float excitementInc = 0;
            actor.setMaxExcitement(0);
            std::vector<float> stimulationValues;
            for (auto& action : m_currentNode->actions) {
                if (action.actor == position) {
                    float stimulation = action.attributes->getActorStimulation(actor.getActor());
                    if (stimulation != 0) {
                        stimulationValues.push_back(stimulation);
                        float maxStim = action.attributes->getActorMaxStimulation(actor.getActor());
                        if (maxStim > actor.getMaxExcitement()) {
                            actor.setMaxExcitement(maxStim);
                        }
                    }
                }
                if (action.target == position) {
                    float stimulation = action.attributes->getTargetStimulation(actor.getActor());
                    if (stimulation != 0) {
                        stimulationValues.push_back(stimulation);
                        float maxStim = action.attributes->getTargetMaxStimulation(actor.getActor());
                        if (maxStim > actor.getMaxExcitement()) {
                            actor.setMaxExcitement(maxStim);
                        }
                    }
                }
                if (action.performer == position) {
                    float stimulation = action.attributes->getPerformerStimulation(actor.getActor());
                    if (stimulation != 0) {
                        stimulationValues.push_back(stimulation);
                        float maxStim = action.attributes->getPerformerMaxStimulation(actor.getActor());
                        if (maxStim > actor.getMaxExcitement()) {
                            actor.setMaxExcitement(maxStim);
                        }
                    }
                }
            }

            switch (stimulationValues.size()) {
                case 0:
                    excitementInc = 0;
                    break;
                case 1:
                    excitementInc = stimulationValues[0];
                    break;
                default: {
                    std::sort(stimulationValues.begin(), stimulationValues.end(), std::greater<float>());
                    excitementInc = stimulationValues[0];
                    for (int i = 1; i < stimulationValues.size(); i++) {
                        excitementInc += stimulationValues[i] * 0.1;
                    }
                } break;
            }

            actor.setBaseExcitementInc(excitementInc);
            if (excitementInc <= 0) {
                actor.setMaxExcitement(0);
            }


            // --- undressing --- //
            if (position < m_currentNode->actors.size() && !m_currentNode->actors[position].noStrip) {
                if (MCM::MCMTable::undressMidScene() && m_currentNode->doFullStrip(position)) {
                    actor.undress();
                    actor.removeWeapons();
                } else if (MCM::MCMTable::partialUndressing()) {
                    uint32_t slotMask = m_currentNode->getStrippingMask(position);
                    if (slotMask != 0) {
                        actor.undressPartial(slotMask);
                        if ((slotMask & MCM::MCMTable::removeWeaponsWithSlot()) != 0) {
                            actor.removeWeapons();
                        }
                    }
                }
            }

            // --- scaling / heel offsets / facial expressions --- //
            if (position < m_currentNode->actors.size()) {
                actor.changeNode(&(m_currentNode->actors[position]), m_currentNode->getFacialExpressions(position), m_currentNode->getOverrideExpressions(position));
            }
        }

        alignActors();

        // sounds
        for (Sound::SoundPlayer* soundPlayer : soundPlayers) {
            delete soundPlayer;
        }
        soundPlayers.clear();

        for (Graph::Action& action : m_currentNode->actions) {
            if (action.muted) {
                continue;
            }

            for (Sound::SoundType* soundType : action.attributes->sounds) {
                ThreadActor* actor = GetActor(action.actor);
                ThreadActor* target = GetActor(action.target);
                if (actor && target) {
                    if (playerThread || !soundType->playPlayerThreadOnly()) {
                        Sound::SoundPlayer* soundPlayer = soundType->create(actor, target);
                        if (soundPlayer) {
                            soundPlayers.push_back(soundPlayer);
                        }
                    }
                }
            }
        }

        if (m_currentNode->speeds.size() == 1) {
            SetSpeed(0);
        } else if (relativeSpeed < 0) {
            SetSpeed(m_currentNode->defaultSpeed);
        } else {
            SetSpeed(static_cast<int>(relativeSpeed * (m_currentNode->speeds.size() - 1) + 0.5));
        }

        if (playerThread) {
            UI::UIState::GetSingleton()->NodeChanged(this, m_currentNode);
        }

        nodeChangedAutoControl();

        auto messaging = SKSE::GetMessagingInterface();

        Messaging::AnimationChangedMessage msg;
        msg.newAnimation = a_node;
        logger::info("Sending animation changed event");
        Messaging::MessagingRegistry::GetSingleton()->SendMessageToListeners(msg);

        if (playerThread) {
            FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_scenechanged", m_currentNode->scene_id, 0);
            FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_scenechanged_" + m_currentNode->scene_id, "", 0);
        }
        FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_thread_scenechanged", m_currentNode->scene_id, m_threadId);
    }

    void Thread::AddActor(RE::Actor* actor) {
        int index = m_actors.size();
        addActorInner(index, actor);
        actor->EvaluatePackage();
    }

    void Thread::RemoveActor() {
        int index = m_actors.size() - 1;
        ThreadActor* actor = GetActor(index);
        removeActorSink(actor->getActor().form);
        actor->free();

        m_actors.erase(index);
    }

    void Thread::addActorInner(int index, RE::Actor* actor) {
        GameAPI::GameActor gameActor = actor;
        gameActor.lock();
        addActorSink(actor);
        m_actors.insert(std::make_pair(index, ThreadActor(this, index, actor)));
        ThreadActor* threadActor = GetActor(index);
        threadActor->initContinue();
        if ((threadFlags & ThreadFlag::UNDRESS) == ThreadFlag::UNDRESS ||  MCM::MCMTable::undressAtStart()) {
            threadActor->undress();
        }
        if (MCM::MCMTable::removeWeaponsAtStart()) {
            threadActor->removeWeapons();
        }
        logger::info("aligning actor");
        alignActor(threadActor, {});
    }

    std::vector<Trait::ActorCondition> Thread::getActorConditions() {
        std::vector<Trait::ActorCondition> conditions;
        for (int i = 0; i < m_actors.size(); i++) {
            conditions.push_back(Trait::ActorCondition::create(GetActor(i)));
        }

        return conditions;
    }

    void Thread::alignActors() {
        rebuildAlignmentKey();
        for (auto& [index, actor] : m_actors) {
            alignActor(&actor, getActorAlignment(index));
        }
    }

    void Thread::alignActor(ThreadActor* threadActor, Alignment::ActorAlignment alignment) {
        float sin = std::sin(center.r);
        float cos = std::cos(center.r);
        
        GameAPI::GamePosition offset{};
        if (Graph::GraphActor* graphActor = threadActor->getGraphActor()) {
            offset = graphActor->offset;
        }

        if (furniture && m_currentNode && m_currentNode->scaleOffsetWithFurniture) {
            offset *= furniture.getScale();
        }

        float x = alignment.offsetX + offset.x;
        float y = alignment.offsetY + offset.y;
        float z = alignment.offsetZ + offset.z;
        float r = alignment.rotation + offset.r;

        threadActor->getActor().lockAtPosition(
            center.x + cos * x + sin * y,
            center.y - sin * x + cos * y,
            center.z + z,
            center.r + MathUtil::toRadians(r));

        threadActor->setScaleMult(alignment.scale * furnitureScaleMult);
        threadActor->setSoSBend(alignment.sosBend);
    }

    void Thread::updateActorAlignment(int index, Alignment::ActorAlignment alignment) {
        alignActor(GetActor(index), alignment);
        Alignment::Alignments::setActorAlignment(alignmentKey, m_currentNode, index, alignment);
    }

    void Thread::loop() {
        //std::shared_lock<std::shared_mutex> readLock(nodeLock);

        if (!playerThread && !GetActor(0)->getActor().isInSameCell(GameAPI::GameActor::getPlayer())) {
            stop();
            return;
        }

        for (auto& actor : m_actors) {
            if (actor.second.getActor().isInCombat() || actor.second.getActor().isDead()) {
                stop();
                return;
            }
        }

        if (stopTimer > 0) {
            if ((stopTimer -= Constants::LOOP_TIME_MILLISECONDS) <= 0) {
                stopFaded();
                return;
            }
        }

        // TODO: Can remove this when we start scenes in c++ with a starting node
        if (!m_currentNode) {
            return;
        }

        loopNavigation();
        loopAutoControl();

        for (auto& actorIt : m_actors) {
            actorIt.second.loop();
        }

        for (Sound::SoundPlayer* player : soundPlayers) {
            player->loop();
        }
    }

    ThreadActor* Thread::GetActor(GameAPI::GameActor a_actor) {
        for (auto& i : m_actors) {
            if (i.second.getActor() == a_actor) return &i.second;
        }
        return nullptr;
    }

    ThreadActor* Thread::GetActor(int a_position) {
        for (auto& i : m_actors) {
            if (i.first == a_position) return &i.second;
        }
        return nullptr;
    }

    int Thread::getActorPosition(GameAPI::GameActor actor) {
        for (auto& i : m_actors) {
            if (i.second.getActor() == actor) return i.first;
        }
        return -1;
    }

    void Thread::SetSpeed(int speed) {
        if (speed < 0) {
            speed = 0;
        } else if (speed >= m_currentNode->speeds.size()) {
            speed = m_currentNode->speeds.size() - 1;
        }
        if (!m_currentNode->isTransition) {
            if (m_currentNode->speeds.size() == 0) {
                relativeSpeed = -1;
            } else {
                relativeSpeed = static_cast<float>(speed) / m_currentNode->speeds.size();
            }
        }
        m_currentNodeSpeed = speed;

        
        for (auto& actorIt : m_actors) {
            if (m_currentNode) {
                if (m_currentNode->speeds.size() > speed) {
                    actorIt.second.playAnimation(m_currentNode->speeds[speed]);

                    // this fixes some face bugs
                    // TODO how to do this with GraphActor?
                    const auto skyrimVM = RE::SkyrimVM::GetSingleton();
                    auto vm = skyrimVM ? skyrimVM->impl : nullptr;
                    if (vm) {
                        RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                        auto args = RE::MakeFunctionArguments<RE::Actor*>(std::move(actorIt.second.getActor().form));
                        vm->DispatchStaticCall("OSKSE", "ApplyNodeOverrides", args, callback);
                    }
                }
            }

            actorIt.second.changeSpeed(speed);
        }

        if (playerThread) {
            UI::UIState::GetSingleton()->SpeedChanged(this, speed);
            FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_animationchanged", m_currentNode->scene_id, speed);
        }
    }

    bool Thread::increaseSpeed() {
        if (m_currentNodeSpeed < (m_currentNode->speeds.size() - 1)) {
            SetSpeed(m_currentNodeSpeed + 1);
            return true;
        }
        return false;
    }

    bool Thread::decreaseSpeed() {
        if (m_currentNodeSpeed > 0) {
            SetSpeed(m_currentNodeSpeed - 1);
            return true;
        }
        return false;
    }


    float Thread::getMaxExcitement() {
        float maxExcitement = 0;
        for (auto& [index, actor] : m_actors) {
            if (actor.getExcitement() > maxExcitement) {
                maxExcitement = actor.getExcitement();
            }
        }
        return maxExcitement;
    }


    void Thread::callEvent(std::string eventName, int actorIndex, int targetIndex, int performerIndex) {
        Graph::Event* graphEvent = Graph::GraphTable::getEvent(eventName);
        if (!graphEvent) {
            return;
        }

        // legacy mod event
        if (playerThread && actorIndex == 0 && targetIndex == 1 && graphEvent->isChildOf(Graph::GraphTable::getEvent("spank"))) {
            FormUtil::sendModEvent(GetActor(1)->getActor().form, "ostim_spank", "", 0);
        }

        ThreadActor* actor = GetActor(actorIndex);
        ThreadActor* target = GetActor(targetIndex);
        ThreadActor* performer = GetActor(performerIndex);

        GameAPI::GameSound sound = graphEvent->getSound();
        if (sound) {
            sound.play(actor->getActor(), MCM::MCMTable::getSoundVolume());
        }

        float cameraShakeDuration = graphEvent->getCameraShakeDuration();
        float cameraShakeStrength = graphEvent->getCameraShakeStrength();
        if (cameraShakeDuration > 0 && cameraShakeStrength > 0) {
            CameraUtil::shakeCamera(cameraShakeStrength, cameraShakeDuration);
        }

        float controllerRumbleDuration = graphEvent->getControllerRumbleDuration();
        float controllerRumbleStrength = graphEvent->getControllerRumbleStrength();
        if (controllerRumbleDuration > 0 && controllerRumbleStrength > 0) {
            ControlUtil::rumbleController(cameraShakeStrength, cameraShakeDuration);
        }

        if (actor) {
            float actorStimulation = graphEvent->getActorStimulation(actor->getActor());
            if (actorStimulation > 0.0 && actor->getExcitement() < graphEvent->getActorMaxStimulation(actor->getActor())) {
                actor->addExcitement(actorStimulation, true);
            }
        }

        if (target) {
            float targetStimulation = graphEvent->getTargetStimulation(target->getActor());
            if (targetStimulation > 0.0 && target->getExcitement() < graphEvent->getTargetMaxStimulation(target->getActor())) {
                target->addExcitement(targetStimulation, true);
            }
        }

        if (performer) {
            float performerStimulation = graphEvent->getPerformerStimulation(performer->getActor());
            if (performerStimulation > 0.0 && performer->getExcitement() < graphEvent->getPerformerMaxStimulation(performer->getActor())) {
                performer->addExcitement(performerStimulation, true);
            }
        }

        if (actor && target) {
            actor->reactToEvent(graphEvent->getActorReactionDelay(), graphEvent, target->getActor(), [](Sound::VoiceSet& voiceSet){return &voiceSet.eventActorReactions;});
            target->reactToEvent(graphEvent->getTargetReactionDelay(), graphEvent, actor->getActor(), [](Sound::VoiceSet& voiceSet){return &voiceSet.eventTargetReactions;});
        }

        if (performer) {
            if (performer == actor) {
                if (target) {
                    performer->reactToEvent(graphEvent->getPerformerReactionDelay(), graphEvent, target->getActor(), [](Sound::VoiceSet& voiceSet){return &voiceSet.eventPerformerReactions;});
                }
            } else {
                if (actor) {
                    performer->reactToEvent(graphEvent->getPerformerReactionDelay(), graphEvent, actor->getActor(), [](Sound::VoiceSet& voiceSet){return &voiceSet.eventPerformerReactions;});
                }
            }
            
        }

        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            int tempID = m_threadId;
            std::string type = graphEvent->id;
            RE::Actor* reActor = actor->getActor().form;
            RE::Actor* reTarget = target->getActor().form;
            RE::Actor* rePerformer = performer->getActor().form;
            auto args = RE::MakeFunctionArguments<>(std::move(tempID), std::move(type), std::move(reActor), std::move(reTarget), std::move(rePerformer));
            vm->DispatchStaticCall("OEvent", "SendOStimEvent", args, callback);
        }
    }


    void Thread::stop() {
        ThreadManager::GetSingleton()->queueThreadStop(m_threadId);
    }

    void Thread::stopFaded() {
        if (playerThread && MCM::MCMTable::useFades()) {
            if (isStopping) {
                return;
            }
            isStopping = true;

            std::thread fadeThread = std::thread([&] {
                GameAPI::GameCamera::fadeToBlack(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(700));
                Thread* thread = ThreadManager::GetSingleton()->getPlayerThread();
                if (thread) {
                    thread->stop();
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(550));
                GameAPI::GameCamera::fadeFromBlack(1);
            });
            fadeThread.detach();
        } else {
            stop();
        }
    }

    void Thread::close() {
        logger::info("closing thread {}", m_threadId);

        for (auto& actorIt : m_actors) {
            actorIt.second.free();
        }

        if (furniture) {
            furniture.enableUse();
            furniture.setOwner(furnitureOwner);
            if (MCM::MCMTable::resetClutter()) {
                // TODO properly use GameObject
                Furniture::resetClutter(furniture.form, MCM::MCMTable::resetClutterRadius() * 100);
            }
        }

        if (playerThread) {
            UI::EndControlledScene();
            GameAPI::GameCamera::endSceneMode(MCM::MCMTable::firstPersonAfterScene());

            RE::INISettingCollection* ini = RE::INISettingCollection::GetSingleton();
            RE::Setting* speed = ini->GetSetting("fFreeCameraTranslationSpeed:Camera");
            if (speed) {
                speed->data.f = freeCamSpeedBefore;
            }

            GameAPI::GameCamera::setWorldFOV(worldFOVbefore);

            if (MCM::MCMTable::customTimeScale() > 0) {
                GameAPI::Game::setTimeScale(timeScaleBefore);
            }
        }
        logger::info("closed thread {}", m_threadId);

        if (playerThread) {
            FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_end", "", -1);
            FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_totalend", "", 0);
        }
        FormUtil::sendModEvent(Util::LookupTable::OSexIntegrationMainQuest, "ostim_thread_end", "", m_threadId);
    }

    void Thread::addActorSink(RE::Actor* a_actor) {
        RE::BSAnimationGraphManagerPtr graphManager;
        a_actor->GetAnimationGraphManager(graphManager);
        if (graphManager) {
		    for (const auto& animationGraph : graphManager->graphs) {
			    animationGraph->GetEventSource<RE::BSAnimationGraphEvent>()->AddEventSink(this);
		    }
	    }
    }

    void Thread::removeActorSink(RE::Actor* a_actor) {
        RE::BSAnimationGraphManagerPtr graphManager;
        a_actor->GetAnimationGraphManager(graphManager);
        if (graphManager) {
		    for (const auto& animationGraph : graphManager->graphs) {
			    animationGraph->GetEventSource<RE::BSAnimationGraphEvent>()->RemoveEventSink(this);
		    }
	    }
    }

    RE::BSEventNotifyControl Thread::ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) {
        if (!a_event || !a_event->holder) {
            return RE::BSEventNotifyControl::kContinue;
        }
        auto actor = const_cast<RE::Actor*>(static_cast<const RE::Actor*>(a_event->holder));

        RE::BSAnimationGraphManagerPtr graphManager;
        actor->GetAnimationGraphManager(graphManager);
        if (!graphManager) {
            return RE::BSEventNotifyControl::kContinue;
        }

        uint32_t activeGraphIdx = graphManager->GetRuntimeData().activeGraph;

        if (graphManager->graphs[activeGraphIdx] && graphManager->graphs[activeGraphIdx].get() != a_eventSource) {
            return RE::BSEventNotifyControl::kContinue;
        }

        std::string tag = a_event->tag.c_str();

        if (tag == "OStimClimax") {
            GetActor(actor)->climax();
        } else if (tag == "OStimEvent") {
            std::vector<std::string> payloadVec = stl::string_split(a_event->payload.c_str(), ',');
            if (payloadVec.size() == 2) {
                int actorIndex = GetActor(actor)->index;
                callEvent(payloadVec[0], actorIndex, std::stoi(payloadVec[1]), actorIndex);
            } else {
                callEvent(payloadVec[0], std::stoi(payloadVec[1]), std::stoi(payloadVec[2]), std::stoi(payloadVec[3]));
            }
        } else if (tag == "OStimUndress") {
            GetActor(actor)->undress();
        } else if (tag == "OStimRedress") {
            GetActor(actor)->redress();
        } else if (tag == "OStimUndressPartial") {
            std::string payload = a_event->payload.c_str();
            int mask = std::stoi(payload, nullptr, 16);
            GetActor(actor)->undressPartial(mask);
        } else if (tag == "OStimRedressPartial"){
            std::string payload = a_event->payload.c_str();
            int mask = std::stoi(payload, nullptr, 16);
            GetActor(actor)->redressPartial(mask);
        } else if (tag == "OStimRemoveWeapons") {
            GetActor(actor)->removeWeapons();
        } else if (tag == "OStimAddWeapons") {
            GetActor(actor)->addWeapons();
        } else if (tag == "OStimEquipObject"){
            GetActor(actor)->equipObject(a_event->payload.c_str());
        } else if (tag == "OStimUnequipObject") {
            GetActor(actor)->unequipObject(a_event->payload.c_str());
        } else if (tag == "OStimSetLooking") {
            std::vector<std::string> payloadVec = stl::string_split(a_event->payload.c_str(), ',');
            std::unordered_map<int, Trait::FaceModifier> eyeballOverride;
            if (!payloadVec.empty()) {
                float value = std::stoi(payloadVec[0]);
                int type = 9;
                if (value < 0) {
                    value *= -1;
                    type = 10;
                }
                eyeballOverride.insert({type, {.type = type, .baseValue = value}});
            }
            if (payloadVec.size() == 2){
                float value = std::stoi(payloadVec[1]);
                int type = 11;
                if (value < 0) {
                    value *= -1;
                    type = 8;
                }
                eyeballOverride.insert({type, {.type = type, .baseValue = value}});
            }
            GetActor(actor)->setLooking(eyeballOverride);
        } else if (tag == "OStimUnsetLooking") {
            GetActor(actor)->unsetLooking();
        } else if (tag == "OStimResetLooking") {
            GetActor(actor)->resetLooking();
        } else if (tag == "OStimPlayExpression") {
            GetActor(actor)->playEventExpression(a_event->payload.c_str());
        } else if (tag == "OStimBendSoS") {
            int bend = std::stoi(a_event->payload.c_str());
            GetActor(actor)->offsetSoSBend(bend);
        } else if (tag == "OStimResetSoS") {
            GetActor(actor)->offsetSoSBend(0);
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    Serialization::OldThread Thread::serialize() {
        Serialization::OldThread oldThread;

        oldThread.threadID = m_threadId;
        if (furniture) {
            oldThread.furniture = furniture;
            oldThread.furnitureOwner = furnitureOwner;
        }

        for (auto& actorIt : m_actors) {
            oldThread.actors.push_back(actorIt.second.serialize());
        }

        return oldThread;
    }

}  // namespace OStim