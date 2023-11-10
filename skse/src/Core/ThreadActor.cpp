#include "ThreadActor.h"

#include "Core.h"
#include "Thread.h"
#include "ThreadManager.h"
#include "GameAPI/Game.h"
#include "GameAPI/GameCamera.h"
#include "Graph/GraphTable.h"
#include "MCM/MCMTable.h"
#include "Sound/SoundTable.h"
#include "Trait/TraitTable.h"
#include "Util/ActorUtil.h"
#include "Util/APITable.h"
#include "Util/CameraUtil.h"
#include "Util/CompatibilityTable.h"
#include "Util/FormUtil.h"
#include "Util/Constants.h"
#include "Util/Globals.h"
#include "Util/LookupTable.h"
#include "Util/ObjectRefUtil.h"
#include "Util/RNGUtil.h"
#include "Util/StringUtil.h"
#include "Util/VectorUtil.h"

namespace OStim {
    ThreadActor::ThreadActor(Thread* thread, int index, GameAPI::GameActor actor) : thread{thread}, index{index}, actor{actor} {
        scaleBefore = actor.getScale();
        positionBefore = actor.getPosition();
        female = actor.isSex(GameAPI::GameSex::FEMALE);
        schlong = Compatibility::CompatibilityTable::hasSchlong(actor);
        isPlayer = actor.isPlayer();
        heelOffset = ActorUtil::getHeelOffset(actor.form);

        excitementMultiplier = female && (!schlong || !MCM::MCMTable::futaUseMaleExcitement()) ? MCM::MCMTable::getFemaleSexExcitementMult() : MCM::MCMTable::getMaleSexExcitementMult();
        loopExcitementDecay = MCM::MCMTable::getExcitementDecayRate() * Constants::LOOP_TIME_SECONDS;

        voiceSet = Sound::SoundTable::getVoiceSet(actor);

        setDialogueCountdown();

        if (schlong) {
            actor.addToFaction(Util::APITable::getSchlongifiedFaction());
        }

        actor.addToFaction(Util::APITable::getExcitementFaction());
        actor.addToFaction(Util::APITable::getTimesClimaxedFaction());
        actor.addToFaction(Util::APITable::getTimeUntilClimaxFaction());
        actor.setFactionRank(Util::APITable::getTimeUntilClimaxFaction(), -1);

        startMoanCooldown();
    }

    void ThreadActor::initContinue() {
        // we're creating a ThreadActor object and then copying it, which will delete the original
        // since we need the object in the callback functor though we need to do it after creating the copy
        // hence the initContinue function

        // TODO properly use GameActor
        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new GetRmHeightCallbackFunctor(this));
            auto args = RE::MakeFunctionArguments(std::move(actor.form), std::move(female));
            vm->DispatchStaticCall("OSKSE", "GetRmScale", args, callback);
        }
    }

    Alignment::ActorKey ThreadActor::getAlignmentKey() {
        return Alignment::ActorKey(female, actor.getHeight() * rmHeight, heelOffsetRemoved ? 0 : heelOffset);
    }


    void ThreadActor::undress() {
        if ((thread->getThreadFlags() & ThreadFlag::NO_UNDRESSING) == ThreadFlag::NO_UNDRESSING) {
            return;
        }

        // TODO properly use GameActor
        if (Util::Globals::usePapyrusUndressing()) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new PapyrusUndressCallbackFunctor(this, false));
                auto args = RE::MakeFunctionArguments(std::move(thread->m_threadId), std::move(actor.form));
                vm->DispatchStaticCall("OUndress", "Undress", args, callback);
            }
            return;
        }

        if (undressed) {
            return;
        }

        auto inventory = actor.form->GetInventory();
        for (const auto& [obj, data] : inventory) {
            auto& [count, entry] = data;

            if (!entry->IsWorn() || !obj->IsArmor() || !FormUtil::canUndress(obj)) {
                continue;
            }
            
            auto armor = obj->As<RE::TESObjectARMO>();
            if (!MCM::MCMTable::undressWigs() && FormUtil::isWig(actor.form, armor)) {
                continue;
            }

            undressedMask |= static_cast<uint32_t>(armor->GetSlotMask());

            undressedItems.push_back(armor);
            ActorUtil::unequipItem(actor.form, obj);
        }

        actor.update3D();

        undressed = true;
    }

    void ThreadActor::undressPartial(uint32_t mask) {
        if ((thread->getThreadFlags() & ThreadFlag::NO_UNDRESSING) == ThreadFlag::NO_UNDRESSING) {
            return;
        }

        // TODO properly use GameActor
        if (Util::Globals::usePapyrusUndressing()) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new PapyrusUndressCallbackFunctor(this, false));
                auto args = RE::MakeFunctionArguments(std::move(thread->m_threadId), std::move(actor.form), std::move(mask));
                vm->DispatchStaticCall("OUndress", "UndressPartial", args, callback);
            }
            return;
        }

        uint32_t filteredMask = (~undressedMask) & mask;
        if (filteredMask == 0) {
            return;
        }

        auto inventory = actor.form->GetInventory();
        for (const auto& [obj, data] : inventory) {
            auto& [count, entry] = data;

            if (!entry->IsWorn() || !obj->IsArmor() || !FormUtil::canUndress(obj)) {
                continue;
            }

            auto armor = obj->As<RE::TESObjectARMO>();
            uint32_t armorMask = static_cast<uint32_t>(armor->GetSlotMask());
            if ((filteredMask & armorMask) == 0 || FormUtil::isWig(actor.form, armor)) {
                continue;
            }

            undressedMask |= armorMask;
            undressedItems.push_back(armor);
            ActorUtil::unequipItem(actor.form, obj);
        }

        actor.update3D();

        // some slots might not have any items equipped in them
        // so to not check them over and over again we add those to the undressedMask
        undressedMask |= mask;
    }

    void ThreadActor::removeWeapons() {
        if (weaponsRemoved) {
            return;
        }

        // TODO properly use GameActor
        rightHand = actor.form->GetEquippedObject(false);
        leftHand = actor.form->GetEquippedObject(true);
        ammo = actor.form->GetCurrentAmmo();

        if (rightHand) {
            ActorUtil::unequipItem(actor.form, rightHand);
        }
        if (leftHand) {
            ActorUtil::unequipItem(actor.form, leftHand);
        }
        if (ammo) {
            ActorUtil::unequipItem(actor.form, ammo);
        }

        weaponsRemoved = true;
    }

    void ThreadActor::redress() {
        // TODO properly use GameActor
        if (Util::Globals::usePapyrusUndressing()) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new PapyrusUndressCallbackFunctor(this, true));
                auto args = RE::MakeFunctionArguments(std::move(thread->m_threadId), std::move(actor.form), std::move(undressedItems));
                vm->DispatchStaticCall("OUndress", "Redress", args, callback);
            }
            return;
        }

        if (undressedMask == 0) {
            return;
        }

        for (auto& armor : undressedItems) {
            ActorUtil::equipItemEx(actor.form, armor);
        }
        undressedItems.clear();
        undressedMask = 0;

        undressed = false;
    }

    void ThreadActor::redressPartial(uint32_t mask) {
        // TODO properly use GameActor
        if (Util::Globals::usePapyrusUndressing()) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new PapyrusUndressCallbackFunctor(this, true));
                auto args = RE::MakeFunctionArguments(std::move(thread->m_threadId), std::move(actor.form), std::move(undressedItems), std::move(mask));
                vm->DispatchStaticCall("OUndress", "RedressPartial", args, callback);
            }
            return;
        }

        uint32_t filteredMask = undressedMask & mask;
        if (filteredMask == 0) {
            return;
        }

        std::vector<RE::TESObjectARMO*>::iterator it = undressedItems.begin();
        while (it != undressedItems.end()) {
            RE::TESObjectARMO* armor = *it;
            uint32_t armorMask = static_cast<uint32_t>(armor->GetSlotMask());
            if ((armorMask & filteredMask) == 0) {
                ++it;
            } else {
                undressedMask &= ~armorMask;
                ActorUtil::equipItemEx(actor.form, armor);

                undressedItems.erase(it);
            }
        }

        if ((undressedMask & filteredMask) != filteredMask) {
            undressed = false;
        }
        undressedMask &= ~mask;
    }

    void ThreadActor::addWeapons() {
        if (!weaponsRemoved) {
            return;
        }

        // TODO properly use GameActor
        if (rightHand) {
            ActorUtil::equipItemEx(actor.form, rightHand, 1);
        }
        if (leftHand) {
            ActorUtil::equipItemEx(actor.form, leftHand, 2);
        }
        if (ammo) {
            ActorUtil::equipItemEx(actor.form, ammo);
        }

        weaponsRemoved = false;
    }

    void ThreadActor::changeNode(Graph::GraphActor* graphActor, std::vector<Trait::FacialExpression*>* nodeExpressions, std::vector<Trait::FacialExpression*>* overrideExpressions) {
        if (this->graphActor) {
            for (GameAPI::GameFaction faction : this->graphActor->factions) {
                actor.removeFromFaction(faction);
            }
        }

        this->graphActor = graphActor;

        sosOffset = 0;

        // heel stuff
        checkHeelOffset();

        // facial expressions
        this->nodeExpressions = nodeExpressions;
        this->overrideExpressions = overrideExpressions;
        updateOverrideExpression();
        eyeballModifierOverride = graphActor->eyeballModifierOverride;
        if (!eyeballModifierOverride.empty() &&
            (!overrideExpression || (overrideExpression->typeMask & Trait::ExpressionType::BALL_MODIFIER) == 0) &&
            (!eventExpression || (eventExpression->typeMask & Trait::ExpressionType::BALL_MODIFIER) == 0)){
            applyEyeballOverride();
        }
        updateUnderlyingExpression();

        // strap-ons
        if (!schlong) {
            if ((graphActor->condition.requirements & Graph::Requirement::PENIS) == Graph::Requirement::PENIS) {
                if (MCM::MCMTable::equipStrapOnIfNeeded()) {
                    equipObject("strapon");
                }
            } else {
                if (MCM::MCMTable::unequipStrapOnIfNotNeeded()) {
                    unequipObject("strapon");
                } else if ((graphActor->condition.requirements & Graph::Requirement::VAGINA) == Graph::Requirement::VAGINA && MCM::MCMTable::unequipStrapOnIfInWay()) {
                    unequipObject("strapon");
                }
            }
        }

        int partnerIndex = thread->getCurrentNode()->getPrimaryPartner(index);
        ThreadActor* partner = thread->GetActor(partnerIndex);
        if (partner) {
            primaryPartner = partner->actor;
        } else {
            primaryPartner = actor;
        }

        for (GameAPI::GameFaction faction : graphActor->factions) {
            actor.addToFaction(faction);
        }

        if (awaitingClimax) {
            climax();
        }
    }

    void ThreadActor::changeSpeed(int speed) {
        this->speed = speed;
        changeSpeedExcitement();
    }

    void ThreadActor::setScaleMult(float scaleMult) {
        this->scaleMult = scaleMult;
        scale();
    }

    void ThreadActor::setSoSBend(int sosBend) {
        this->sosBend = sosBend;
        bendSchlong();
    }

    void ThreadActor::offsetSoSBend(int soSOffset) {
        this->sosOffset = sosOffset;
        bendSchlong();
    }

    void ThreadActor::handleNiNodeUpdate() {
        bendSchlong();
        updateHeelOffset();
    }

    void ThreadActor::loop() {
        loopExcitement();
        loopClimax();

        // expressions
        if (overwriteExpressionCooldown > 0) {
            overwriteExpressionCooldown -= Constants::LOOP_TIME_MILLISECONDS;
            if (overwriteExpressionCooldown <= 0) {
                updateOverrideExpression();
            }
        }

        if (eventExpressionCooldown > 0) {
            eventExpressionCooldown -= Constants::LOOP_TIME_MILLISECONDS;
            if (eventExpressionCooldown <= 0) {
                clearEventExpression();
            }
        }

        if ((underlyingExpressionCooldown -= Constants::LOOP_TIME_MILLISECONDS) < 0) {
            updateUnderlyingExpression();

            // randomly make the actor look down when performing oral
            if (overrideExpression) {
                if (eyeballModifierOverride.size() == 1 && eyeballModifierOverride.contains(8) && eyeballModifierOverride[8].baseValue == 100) {
                    resetLooking();
                } else {
                    if (RNGUtil::uniformInt(0, 4) == 0) {
                        setLooking({{8, {.baseValue = 100}}});
                    }
                }
            }
        }

        // TODO properly use GameActor
        auto faceData = actor.form->GetFaceGenAnimationData();

        if (faceData) {
            if (!modifierUpdaters.empty()) {
                std::vector<int> toDelete;
                for (auto& [key, updater] : modifierUpdaters) {
                    if (updater.delay > 0) {
                        updater.delay -= Constants::LOOP_TIME_MILLISECONDS;
                    } else {
                        if (key >= faceData->modifierKeyFrame.count) {
                            continue;
                        }

                        faceData->modifierKeyFrame.values[key] = updater.step() / 100.0f;
                        if (updater.isDone()) {
                            toDelete.push_back(key);
                        }
                    }
                }

                for (int i : toDelete) {
                    modifierUpdaters.erase(i);
                }

                faceData->modifierKeyFrame.isUpdated = false;
            }

            if (!phonemeUpdaters.empty()) {
                std::vector<int> toDelete;
                for (auto& [key, updater] : phonemeUpdaters) {
                    if (updater.delay > 0) {
                        updater.delay -= Constants::LOOP_TIME_MILLISECONDS;
                    } else {
                        if (key >= faceData->phenomeKeyFrame.count) {
                            continue;
                        }

                        if (actor.isTalking()) {
                            updater.step();
                        } else {
                            faceData->phenomeKeyFrame.values[key] = updater.step() / 100.0f;
                        }
                        if (updater.isDone()) {
                            toDelete.push_back(key);
                        }
                    }
                }

                for (int i : toDelete) {
                    phonemeUpdaters.erase(i);
                }

                faceData->phenomeKeyFrame.isUpdated = false;
            }
        }

        loopSound();

        // equip objects
        for (auto& [type, object] : equipObjects) {
            if (object.variantDuration > 0) {
                object.variantDuration -= Constants::LOOP_TIME_MILLISECONDS;
                if (object.variantDuration <= 0) {
                    object.unsetVariant(actor);
                }
            }
        }
    }

    void ThreadActor::checkHeelOffset() {
        if (!graphActor) {
            return;
        }

        applyHeelOffset(!graphActor->feetOnGround);
    }

    void ThreadActor::applyHeelOffset(bool remove) {
        if (remove == heelOffsetRemoved) {
            return;
        }

        if (heelOffset == 0) {
            heelOffsetRemoved = remove;
            return;
        }

        // TODO properly use GameActor here
        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto args = RE::MakeFunctionArguments(std::move(actor.form), std::move(heelOffset), std::move(!remove), std::move(remove), std::move(female));
            vm->DispatchStaticCall("OSKSE", "UpdateHeelOffset", args, callback);
        }

        heelOffsetRemoved = remove;
    }

    void ThreadActor::updateHeelOffset() {
        int oldOffset = heelOffset;
        // TODO properly use GameActor
        heelOffset = ActorUtil::getHeelOffset(actor.form);

        if (oldOffset == heelOffset) {
            return;
        }

        if (!heelOffsetRemoved) {
            scale();
            if (MCM::MCMTable::groupAlignmentByHeels()) {
                thread->alignActors();
            }
            return;
        }

        // TODO properly use GameActor
        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto args = RE::MakeFunctionArguments(std::move(actor.form), std::move(heelOffset), std::move(oldOffset != 0), std::move(heelOffset != 0), std::move(female));
            vm->DispatchStaticCall("OSKSE", "UpdateHeelOffset", args, callback);
        }
    }

    void ThreadActor::updateUnderlyingExpression() {
        // TODO no longer use faction here
        if (!Trait::TraitTable::areFacialExpressionsBlocked(actor.form)) {
            int mask = 0;
            if (underlyingExpression) {
                mask = underlyingExpression->typeMask;
            }
            underlyingExpression = VectorUtil::randomElement(nodeExpressions)->getGenderExpression(female);
            mask |= underlyingExpression->typeMask;
            if (!eyeballModifierOverride.empty()) {
                mask &= ~Trait::ExpressionType::BALL_MODIFIER;
            }
            if (eventExpression) {
                mask &= ~eventExpression->typeMask;
            }
            if (overrideExpression) {
                mask &= ~overrideExpression->typeMask;
            }
            applyExpression(underlyingExpression, mask, 1);
        }
        underlyingExpressionCooldown = RNGUtil::uniformInt(MCM::MCMTable::getExpressionDurationMin(), MCM::MCMTable::getExpressionDurationMax());
    }

    void ThreadActor::updateOverrideExpression() {
        int mask = 0;
        if (overrideExpression) {
            mask = overrideExpression->typeMask;
        }
        if (overrideExpressions) {
            overrideExpression = VectorUtil::randomElement(overrideExpressions)->getGenderExpression(female);
            mask &= ~overrideExpression->typeMask;
            applyExpression(overrideExpression, overrideExpression->typeMask, 5);
            overwriteExpressionCooldown = RNGUtil::uniformInt(MCM::MCMTable::getExpressionDurationMin(), MCM::MCMTable::getExpressionDurationMax());
        } else {
            overrideExpression = nullptr;
            overwriteExpressionCooldown = -1;
        }
        
        wakeExpressions(mask);
    }

    void ThreadActor::setEventExpression(std::string expression) {
        StringUtil::toLower(&expression);
        std::vector<Trait::FacialExpression*>* expressions = Trait::TraitTable::getExpressionsForEvent(expression);
        if (!expressions) {
            return;
        }

        setEventExpression(VectorUtil::randomElement(expressions));
    }

    void ThreadActor::setEventExpression(Trait::FacialExpression* expression) {
        int mask = 0;
        if (eventExpression) {
            mask = eventExpression->typeMask;
        }
        eventExpression = expression->getGenderExpression(female);
        if (overrideExpression) {
            applyExpression(eventExpression, eventExpression->typeMask & ~overrideExpression->typeMask, 3);
            mask &= ~eventExpression->typeMask & ~overrideExpression->typeMask;
        } else {
            applyExpression(eventExpression, eventExpression->typeMask, 3);
            mask &= ~eventExpression->typeMask;
        }
        
        wakeExpressions(mask);
    }

    void ThreadActor::clearEventExpression() {
        if (eventExpression) {
            int mask = eventExpression->typeMask;
            if (overrideExpression) {
                mask &= ~overrideExpression->typeMask;
            }
            eventExpression = nullptr;
            wakeExpressions(mask);
        }
    }

    void ThreadActor::playEventExpression(std::string expression) {
        StringUtil::toLower(&expression);
        std::vector<Trait::FacialExpression*>* expressions = Trait::TraitTable::getExpressionsForEvent(expression);
        if (!expressions) {
            return;
        }

        playEventExpression(VectorUtil::randomElement(expressions));
    }

    void ThreadActor::playEventExpression(Trait::FacialExpression* expression) {
        setEventExpression(expression);
        eventExpressionCooldown = expression->getDuration(actor) * 1000;
    }

    bool ThreadActor::hasExpressionOverride() {
        return overrideExpressions;
    }

    void ThreadActor::setLooking(std::unordered_map<int, Trait::FaceModifier> eyeballOverride) {
        eyeballModifierOverride = eyeballOverride;
        checkForEyeballOverride();
    }

    void ThreadActor::unsetLooking() {
        eyeballModifierOverride.clear();
        checkForEyeballOverride();
    }

    void ThreadActor::resetLooking() {
        if (graphActor) {
            eyeballModifierOverride = graphActor->eyeballModifierOverride;
        } else {
            eyeballModifierOverride.clear();
        }
        checkForEyeballOverride();
    }

    void ThreadActor::wakeExpressions(int mask) {
        if (mask == 0) {
            return;
        }

        if (eventExpression) {
            applyExpression(eventExpression, mask & eventExpression->typeMask, 3);
            mask &= ~eventExpression->typeMask;
        }

        if ((mask & Trait::ExpressionType::BALL_MODIFIER) == Trait::ExpressionType::BALL_MODIFIER && graphActor && !eyeballModifierOverride.empty()) {
            applyEyeballOverride();
            mask &= ~Trait::ExpressionType::BALL_MODIFIER;
        }
        
        if (underlyingExpression) {
            applyExpression(underlyingExpression, mask, 1);
        }
    }

    void ThreadActor::applyExpression(Trait::GenderExpression* expression, int mask, int updateSpeed) {
        if (mask == 0) {
            return;
        }

        // TODO properly use GameActor
        auto faceData = actor.form->GetFaceGenAnimationData();
        if (!faceData) {
            return;
        }

        // expression
        if ((mask & Trait::ExpressionType::EXPRESSION) == Trait::ExpressionType::EXPRESSION) {
            if (auto value = expression->expression.calculate(thread->getRelativeSpeed(), excitement)) {
                faceData->exprOverride = false;
                faceData->SetExpressionOverride(expression->expression.type, static_cast<float>(value) / 100.0f);
                faceData->exprOverride = true;
            }
        }

        // modifiers
        if ((mask & Trait::ExpressionType::LID_MODIFIER) == Trait::ExpressionType::LID_MODIFIER) {
            for (int i : Trait::eyelidModifierTypes) {
                if (i >= faceData->modifierKeyFrame.count) {
                    continue;
                }
                
                int current = faceData->modifierKeyFrame.values[i] * 100;
                int goal = 0;
                int delay = 0;
                auto iter = expression->eyelidModifiers.find(i);
                if (iter != expression->eyelidModifiers.end()) {
                    goal = iter->second.calculate(thread->getRelativeSpeed(), excitement);
                    delay = iter->second.randomizeDelay();
                }
                if (current == goal) {
                    modifierUpdaters.erase(i);
                } else {
                    modifierUpdaters[i] = {.delay = delay, .current = current, .goal = goal, .speed = updateSpeed};
                }
            }
        }

        if ((mask & Trait::ExpressionType::BROW_MODIFIER) == Trait::ExpressionType::BROW_MODIFIER) {
            for (int i : Trait::eyebrowModifierTypes) {
                if (i >= faceData->modifierKeyFrame.count) {
                    continue;
                }

                int current = faceData->modifierKeyFrame.values[i] * 100;
                int goal = 0;
                int delay = 0;
                auto iter = expression->eyebrowModifiers.find(i);
                if (iter != expression->eyebrowModifiers.end()) {
                    goal = iter->second.calculate(thread->getRelativeSpeed(), excitement);
                    delay = iter->second.randomizeDelay();
                }
                if (current == goal) {
                    modifierUpdaters.erase(i);
                } else {
                    modifierUpdaters[i] = {.delay = delay, .current = current, .goal = goal, .speed = updateSpeed};
                }
            }
        }

        if ((mask & Trait::ExpressionType::BALL_MODIFIER) == Trait::ExpressionType::BALL_MODIFIER) {
            for (int i : Trait::eyeballModifierTypes) {
                if (i >= faceData->modifierKeyFrame.count) {
                    continue;
                }

                int current = faceData->modifierKeyFrame.values[i] * 100;
                int goal = 0;
                int delay = 0;
                auto iter = expression->eyeballModifiers.find(i);
                if (iter != expression->eyeballModifiers.end()) {
                    goal = iter->second.calculate(thread->getRelativeSpeed(), excitement);
                    delay = iter->second.randomizeDelay();
                }
                if (current == goal) {
                    modifierUpdaters.erase(i);
                } else {
                    modifierUpdaters[i] = {.delay = delay, .current = current, .goal = goal, .speed = 5};
                }
            }
        }

        // phonemes
        if ((mask & Trait::ExpressionType::PHONEME) == Trait::ExpressionType::PHONEME) {
            for (int i = 0; i < 14; i++) {
                if (i >= faceData->phenomeKeyFrame.count) {
                    continue;
                }

                int current = faceData->phenomeKeyFrame.values[i] * 100;
                int goal = 0;
                int delay = 0;
                auto iter = expression->phonemes.find(i);
                if (iter != expression->phonemes.end()) {
                    goal = iter->second.calculate(thread->getRelativeSpeed(), excitement);
                    delay = iter->second.randomizeDelay();
                }
                if (current == goal) {
                    phonemeUpdaters.erase(i);
                } else {
                    phonemeUpdaters[i] = {.delay = delay, .current = current, .goal = goal, .speed = updateSpeed};
                }
            }

            if (excitement >= expression->phonemeObjectThreshold) {
                for (std::string object : phonemeObjects) {
                    if (!VectorUtil::contains(expression->phonemeObjects, object)) {
                        unequipObject(object);
                    }
                }

                for (std::string object : expression->phonemeObjects) {
                    if (!VectorUtil::contains(phonemeObjects, object)) {
                        equipObject(object);
                    }
                }

                phonemeObjects = expression->phonemeObjects;
            } else {
                for (std::string object : phonemeObjects) {
                    unequipObject(object);
                }
            }
        }
    }

    void ThreadActor::checkForEyeballOverride() {
        if ((!overrideExpression || (overrideExpression->typeMask & Trait::ExpressionType::BALL_MODIFIER) == 0) &&
            (!eventExpression || (eventExpression->typeMask & Trait::ExpressionType::BALL_MODIFIER) == 0)) {
            if (eyeballModifierOverride.empty()) {
                wakeExpressions(Trait::ExpressionType::BALL_MODIFIER);
            } else {
                applyEyeballOverride();
            }
        }
    }

    void ThreadActor::applyEyeballOverride() {
        // TODO properly use GameActor
        auto faceData = actor.form->GetFaceGenAnimationData();

        if (!faceData) {
            return;
        }

        for (int i : Trait::eyeballModifierTypes) {
            if (i >= faceData->modifierKeyFrame.count) {
                continue;
            }

            int current = faceData->modifierKeyFrame.values[i] * 100;
            int goal = 0;
            int delay = 0;
            auto iter = eyeballModifierOverride.find(i);
            if (iter != eyeballModifierOverride.end()) {
                goal = iter->second.calculate(thread->getRelativeSpeed(), excitement);
                delay = iter->second.randomizeDelay();
            }
            if (current == goal) {
                modifierUpdaters.erase(i);
            } else {
                modifierUpdaters[i] = {.delay = delay, .current = current, .goal = goal, .speed = 5};
            }
        }
    }

    bool ThreadActor::equipObject(std::string type) {
        auto iter = equipObjects.find(type);
        if (iter != equipObjects.end()) {
            iter->second.equip(actor);
            return true;
        }
        
        Trait::EquipObject* object = Trait::TraitTable::getEquipObject(actor, type);
        if (object) {
            equipObjects[type] = {.actor = actor, .object = object};
            equipObjects[type].equip(actor);

            return true;
        }

        return false;
    }

    void ThreadActor::unequipObject(std::string type) {
        auto iter = equipObjects.find(type);
        if (iter != equipObjects.end()) {
            iter->second.unequip(actor);
        }
    }

    bool ThreadActor::isObjectEquipped(std::string type) {
        auto iter = equipObjects.find(type);
        if (iter != equipObjects.end()) {
            return iter->second.equipped;
        }
        return false;
    }

    bool ThreadActor::setObjectVariant(std::string type, std::string variant, int duration) {
        auto iter = equipObjects.find(type);
        if (iter != equipObjects.end()) {
            return iter->second.setVariant(actor, variant, duration);
        }

        Trait::EquipObject* object = Trait::TraitTable::getEquipObject(actor, type);
        if (object) {
            equipObjects[type] = {.actor = actor, .object = object};
            return equipObjects[type].setVariant(actor, variant, duration);
        }

        return false;
    }

    void ThreadActor::unsetObjectVariant(std::string type) {
        auto iter = equipObjects.find(type);
        if (iter != equipObjects.end()) {
            iter->second.unsetVariant(actor);
        }
    }


    void ThreadActor::free() {
        logger::info("freeing actor {}-{}: {}", thread->m_threadId, index, actor.getName());

        if (this->graphActor) {
            for (GameAPI::GameFaction faction : this->graphActor->factions) {
                actor.removeFromFaction(faction);
            }
        }

        for (auto& [type, object] : equipObjects) {
            object.unequip(actor);
            object.removeItems(actor);
        }

        // TODO properly use GameActor
        if (MCM::MCMTable::animateRedress() && !isPlayer) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                std::vector<RE::TESForm*> weapons = {rightHand, leftHand, ammo};
                auto args = RE::MakeFunctionArguments(std::move(actor.form), std::move(female), std::move(undressedItems), std::move(weapons));
                vm->DispatchStaticCall("OUndress", "AnimateRedress", args, callback);
            }
        } else {
            if (Util::Globals::usePapyrusUndressing()) {
                // this object will be destroyed before papyrus redressing is done
                // so for this case we need to invoke Redress without a callback here
                const auto skyrimVM = RE::SkyrimVM::GetSingleton();
                auto vm = skyrimVM ? skyrimVM->impl : nullptr;
                if (vm) {
                    RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                    auto args = RE::MakeFunctionArguments(std::move(thread->m_threadId), std::move(actor.form), std::move(undressedItems));
                    vm->DispatchStaticCall("OUndress", "Redress", args, callback);
                }
            } else {
                redress();
            }
            addWeapons();
        }

        applyHeelOffset(false);

        actor.setScale(scaleBefore);
        
        freeActor(actor, false);

        logger::info("resetting position");
        if (MCM::MCMTable::resetPosition()) {
            // running this immediately can cause game freezes, running it in a task works fine, though
            // for me freezes were only in a threesome, but some users reported it in twosomes or other scenarios
            GameAPI::GameActor tempActor = actor;
            GameAPI::GamePosition tempPosition = positionBefore;
            SKSE::GetTaskInterface()->AddTask([tempActor, tempPosition] {
                tempActor.setPosition(tempPosition);
            });
        }

        // no need to do this in ActorUtil::free since facedata isn't written into the savefile anyways
        // TODO properly use GameActor
        auto faceData = actor.form->GetFaceGenAnimationData();
        if (faceData) {
            faceData->ClearExpressionOverride();
            faceData->Reset(0.0, true, true, true, false);
        }

        logger::info("freed actor {}-{}: {}", thread->m_threadId, index, actor.getName());

        if (!actor.isPlayer() && thread->getActors().size() > 1) {
            // TODO: GameActor??
            if (voiceSet.postSceneDialogue) {
                const auto skyrimVM = RE::SkyrimVM::GetSingleton();
                auto vm = skyrimVM ? skyrimVM->impl : nullptr;
                if (vm) {
                    RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                    auto args = RE::MakeFunctionArguments(std::move(actor.form), std::move(primaryPartner.form), std::move(voiceSet.postSceneDialogue.form));
                    vm->DispatchStaticCall("OSKSE", "SayPostDialogue", args, callback);
                }
            }
        }
    }

    void ThreadActor::papyrusUndressCallback(std::vector<RE::TESObjectARMO*> items) {
        for (RE::TESObjectARMO* item : items) {
            if (VectorUtil::contains(undressedItems, item)) {
                continue;
            }

            undressedItems.push_back(item);
        }
    }

    void ThreadActor::papyrusRedressCallback(std::vector<RE::TESObjectARMO*> items) {
        std::erase_if(undressedItems, [items](RE::TESObjectARMO* item){return VectorUtil::contains(items, item);});
    }

    void ThreadActor::GetRmHeightCallbackFunctor::setRmHeight(float height) {
        if (height == 1) {
            return;
        }

        threadActor->rmHeight = height;
        float currentScale = threadActor->actor.getScale();
        threadActor->actor.setScale(currentScale / height);

        if (MCM::MCMTable::groupAlignmentByHeight()) {
            threadActor->thread->alignActors();
        }
    }

    Serialization::OldThreadActor ThreadActor::serialize() {
        Serialization::OldThreadActor oldThreadActor;

        oldThreadActor.actor = actor.form;

        for (auto& [type, object] : equipObjects) {
            for (RE::TESObjectARMO* item : object.toRemove) {
                oldThreadActor.equipObjects.push_back(item);
            }
        }

        oldThreadActor.factions = graphActor->factions;

        return oldThreadActor;
    }
}