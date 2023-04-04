#include "ThreadActor.h"

#include "Core.h"
#include "ThreadManager.h"
#include "Graph/LookupTable.h"
#include "Trait/TraitTable.h"
#include "Util/ActorUtil.h"
#include "Util/FormUtil.h"
#include "Util/Constants.h"
#include "Util/MCMTable.h"
#include "Util/ObjectRefUtil.h"
#include "Trait/TraitTable.h"
#include "Util/VectorUtil.h"

namespace OStim {
    ThreadActor::ThreadActor(int threadId, RE::Actor* actor) : threadId{threadId}, actor{actor} {
        scaleBefore = actor->GetReferenceRuntimeData().refScale / 100.0f;
        isFemale = actor->GetActorBase()->GetSex() == RE::SEX::kFemale;
        hasSchlong = Trait::TraitTable::hasSchlong(actor);
        isPlayer = actor == RE::PlayerCharacter::GetSingleton();
        Trait::TraitTable::addToExcitementFaction(actor);
        heelOffset = ActorUtil::getHeelOffset(actor);

        baseExcitementMultiplier = isFemale ? MCM::MCMTable::getFemaleSexExcitementMult() : MCM::MCMTable::getMaleSexExcitementMult();
        loopExcitementDecay = MCM::MCMTable::getExcitementDecayRate() * Constants::LOOP_TIME_SECONDS;
    }

    void ThreadActor::initContinue() {
        // for some reason the "this" keyword is not properly giving a pointer to this object when used in the constructor
        // so we do everything that uses pointers in this initContinue method
        if (REL::Module::GetRuntime() == REL::Module::Runtime::AE) {
            auto nioInterface = Graph::LookupTable::getNiTransformInterface();
            if (nioInterface->HasNodeTransformScale(actor, false, isFemale, "NPC", "RSMPlugin")) {
                rmHeight = nioInterface->GetNodeTransformScale(actor, false, isFemale, "NPC", "RSMPlugin");
            }
        } else {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new GetRmHeightCallbackFunctor(this));
                auto args = RE::MakeFunctionArguments(std::move(actor), std::move(isFemale));
                vm->DispatchStaticCall("OSKSE", "GetRmScale", args, callback);
            }
        }
    }

    Alignment::ActorKey ThreadActor::getAlignmentKey() {
        return Alignment::ActorKey(isFemale, actor->GetActorBase()->GetHeight() * rmHeight, heelOffsetRemoved ? 0 : heelOffset);
    }

    void ThreadActor::undress() {
        if (MCM::MCMTable::usePapyrusUndressing()) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new PapyrusUndressCallbackFunctor(this, false));
                auto args = RE::MakeFunctionArguments(std::move(threadId), std::move(actor));
                vm->DispatchStaticCall("OUndress", "Undress", args, callback);
            }
            return;
        }

        if (undressed) {
            return;
        }

        auto inventory = actor->GetInventory();
        for (const auto& [obj, data] : inventory) {
            auto& [count, entry] = data;

            if (!entry->IsWorn() || !obj->IsArmor() || !FormUtil::canUndress(obj)) {
                continue;
            }
            
            auto armor = obj->As<RE::TESObjectARMO>();
            if (FormUtil::isWig(actor, armor)) {
                continue;
            }

            undressedMask |= static_cast<uint32_t>(armor->GetSlotMask());

            undressedItems.push_back(armor);
            ActorUtil::unequipItem(actor, obj);
        }

        ActorUtil::queueNiNodeUpdate(actor);

        undressed = true;
    }

    void ThreadActor::undressPartial(uint32_t mask) {
        if (MCM::MCMTable::usePapyrusUndressing()) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new PapyrusUndressCallbackFunctor(this, false));
                auto args = RE::MakeFunctionArguments(std::move(threadId), std::move(actor), std::move(mask));
                vm->DispatchStaticCall("OUndress", "UndressPartial", args, callback);
            }
            return;
        }

        uint32_t filteredMask = (~undressedMask) & mask;
        if (filteredMask == 0) {
            return;
        }

        auto inventory = actor->GetInventory();
        for (const auto& [obj, data] : inventory) {
            auto& [count, entry] = data;

            if (!entry->IsWorn() || !obj->IsArmor() || !FormUtil::canUndress(obj)) {
                continue;
            }

            auto armor = obj->As<RE::TESObjectARMO>();
            uint32_t armorMask = static_cast<uint32_t>(armor->GetSlotMask());
            if ((filteredMask & armorMask) == 0 || FormUtil::isWig(actor, armor)) {
                continue;
            }

            undressedMask |= armorMask;
            undressedItems.push_back(armor);
            ActorUtil::unequipItem(actor, obj);
        }

        ActorUtil::queueNiNodeUpdate(actor);

        // some slots might not have any items equipped in them
        // so to not check them over and over again we add those to the undressedMask
        undressedMask |= mask;
    }

    void ThreadActor::removeWeapons() {
        if (weaponsRemoved) {
            return;
        }

        rightHand = actor->GetEquippedObject(false);
        leftHand = actor->GetEquippedObject(true);
        ammo = actor->GetCurrentAmmo();

        if (rightHand) {
            ActorUtil::unequipItem(actor, rightHand);
        }
        if (leftHand) {
            ActorUtil::unequipItem(actor, leftHand);
        }
        if (ammo) {
            ActorUtil::unequipItem(actor, ammo);
        }

        weaponsRemoved = true;
    }

    void ThreadActor::redress() {
        if (MCM::MCMTable::usePapyrusUndressing()) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new PapyrusUndressCallbackFunctor(this, true));
                auto args = RE::MakeFunctionArguments(std::move(threadId), std::move(actor), std::move(undressedItems));
                vm->DispatchStaticCall("OUndress", "Redress", args, callback);
            }
            return;
        }

        if (undressedMask == 0) {
            return;
        }

        for (auto& armor : undressedItems) {
            ActorUtil::equipItemEx(actor, armor);
        }
        undressedItems.clear();
        undressedMask = 0;

        undressed = false;
    }

    void ThreadActor::redressPartial(uint32_t mask) {
        if (MCM::MCMTable::usePapyrusUndressing()) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback(new PapyrusUndressCallbackFunctor(this, true));
                auto args = RE::MakeFunctionArguments(std::move(threadId), std::move(actor), std::move(undressedItems), std::move(mask));
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
                ActorUtil::equipItemEx(actor, armor);

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

        if (rightHand) {
            ActorUtil::equipItemEx(actor, rightHand, 1);
        }
        if (leftHand) {
            ActorUtil::equipItemEx(actor, leftHand, 2);
        }
        if (ammo) {
            ActorUtil::equipItemEx(actor, ammo);
        }

        weaponsRemoved = false;
    }

    void ThreadActor::changeNode(Graph::Actor* graphActor, std::vector<Trait::FacialExpression*>* nodeExpressions, std::vector<Trait::FacialExpression*>* overrideExpressions) {
        this->graphActor = graphActor;

        // heel stuff
        checkHeelOffset();
        scale();

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
        if (!hasSchlong) {
            if ((graphActor->requirements & Graph::Requirement::PENIS) == Graph::Requirement::PENIS) {
                if (MCM::MCMTable::equipStrapOnIfNeeded()) {
                    equipObject("strapon");
                }
            } else {
                if (MCM::MCMTable::unequipStrapOnIfNotNeeded()) {
                    unequipObject("strapon");
                } else if ((graphActor->requirements & Graph::Requirement::VAGINA) == Graph::Requirement::VAGINA && MCM::MCMTable::unequipStrapOnIfInWay()) {
                    unequipObject("strapon");
                }
            }
        }
    }

    void ThreadActor::changeSpeed(int speed) {
        this->speed = speed;
    }

    void ThreadActor::setSoSBend(int sosBend) {
        this->sosBend = sosBend;
        bendSchlong();
    }

    void ThreadActor::handleNiNodeUpdate() {
        bendSchlong();
        updateHeelOffset();
    }

    void ThreadActor::loop() {
        if (excitement > maxExcitement) {
            if (excitementDecayCooldown > 0) {
                excitementDecayCooldown -= Constants::LOOP_TIME_MILLISECONDS;
            } else {
                excitement -= loopExcitementDecay;
                if (excitement < maxExcitement) {
                    excitement = maxExcitement;
                }
                Trait::TraitTable::setExcitement(actor, excitement);
            }
        } else {  // increase excitement
            excitement += loopExcitementInc;
            if (excitement > maxExcitement) {
                excitement = maxExcitement;
            }
            Trait::TraitTable::setExcitement(actor, excitement);
            excitementDecayCooldown = MCM::MCMTable::getExcitementDecayGracePeriod();
        }

        // expressions
        if (overwriteExpressionCooldown > 0) {
            overwriteExpressionCooldown -= Constants::LOOP_TIME_MILLISECONDS;
            if (overwriteExpressionCooldown <= 0) {
                updateOverrideExpression();
            }
        }
        if ((underlyingExpressionCooldown -= Constants::LOOP_TIME_MILLISECONDS) < 0) {
            updateUnderlyingExpression();

            // randomly make the actor look down when performing oral
            if (overrideExpression) {
                if (eyeballModifierOverride.size() == 1 && eyeballModifierOverride.contains(8) && eyeballModifierOverride[8].baseValue == 100) {
                    resetLooking();
                } else {
                    if (std::uniform_int_distribution<int>(0, 4)(Constants::RNG) == 0) {
                        setLooking({{8, {.baseValue = 100}}});
                    }
                }
            }
        }

        auto faceData = actor->GetFaceGenAnimationData();

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

                        faceData->phenomeKeyFrame.values[key] = updater.step() / 100.0f;
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
        } else {
            logger::warn("no face data on actor {}", actor->GetDisplayFullName());
        }

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

    void ThreadActor::bendSchlong() {
        if (!MCM::MCMTable::isSchlongBendingDisabled()) {
            actor->NotifyAnimationGraph("SOSBend" + std::to_string(sosBend));
        }
    }

    void ThreadActor::scale() {
        if (MCM::MCMTable::isScalingDisabled() || !graphActor) {
            return;
        }

        float newScale = graphActor->scale / (actor->GetActorBase()->GetHeight() * rmHeight);
        if (!heelOffsetRemoved && heelOffset != 0) {
            newScale *= graphActor->scaleHeight / (graphActor->scaleHeight + heelOffset);
        }

        // setscale resets 3BA physics, so we don't do it if the actor already has the desired scale
        if (static_cast<int>(newScale * 100) != actor->GetReferenceRuntimeData().refScale) {
            ActorUtil::setScale(actor, newScale);
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

        // the NiTransformInterface has only been added to RaceMenu after the AE update
        // so for SE we have to invoke Papyrus here :^(
        if (REL::Module::GetRuntime() == REL::Module::Runtime::AE) {
            auto nioInterface = Graph::LookupTable::getNiTransformInterface();
            if (remove) {
                // we are adding a second node transform with a different key to counter out the existing one, thereby
                // "removing" the heel offset
                SKEE::INiTransformInterface::Position offset{};
                offset.z = -heelOffset;
                nioInterface->AddNodeTransformPosition(actor, false, isFemale, "NPC", "OStim", offset);
            } else {
                nioInterface->RemoveNodeTransformPosition(actor, false, isFemale, "NPC", "OStim");
            }
            nioInterface->UpdateNodeTransforms(actor, false, isFemale, "NPC");
        } else {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                auto args = RE::MakeFunctionArguments(std::move(actor), std::move(heelOffset), std::move(!remove), std::move(remove), std::move(isFemale));
                vm->DispatchStaticCall("OSKSE", "UpdateHeelOffset", args, callback);
            }
        }

        heelOffsetRemoved = remove;
    }

    void ThreadActor::updateHeelOffset() {
        int oldOffset = heelOffset;
        heelOffset = ActorUtil::getHeelOffset(actor);

        if (oldOffset == heelOffset) {
            return;
        }

        if (!heelOffsetRemoved) {
            scale();
            if (MCM::MCMTable::groupAlignmentByHeels()) {
                ThreadManager::GetSingleton()->GetThread(threadId)->alignActors();
            }
            return;
        }

        if (REL::Module::GetRuntime() == REL::Module::Runtime::AE) {
            auto nioInterface = Graph::LookupTable::getNiTransformInterface();
            if (oldOffset != 0) {
                nioInterface->RemoveNodeTransformPosition(actor, false, isFemale, "NPC", "OStim");
            }
            if (heelOffset != 0) {
                SKEE::INiTransformInterface::Position offset{};
                offset.z = -heelOffset;
                nioInterface->AddNodeTransformPosition(actor, false, isFemale, "NPC", "OStim", offset);
            }
            nioInterface->UpdateNodeTransforms(actor, false, isFemale, "NPC");
        } else {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                auto args = RE::MakeFunctionArguments(std::move(actor), std::move(heelOffset), std::move(oldOffset != 0), std::move(heelOffset != 0), std::move(isFemale));
                vm->DispatchStaticCall("OSKSE", "UpdateHeelOffset", args, callback);
            }
        }
    }

    void ThreadActor::updateUnderlyingExpression() {
        if (!Trait::TraitTable::areFacialExpressionsBlocked(actor)) {
            int mask = 0;
            if (underlyingExpression) {
                mask = underlyingExpression->typeMask;
            }
            underlyingExpression = VectorUtil::randomElement(nodeExpressions)->getGenderExpression(isFemale);
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
        underlyingExpressionCooldown = std::uniform_int_distribution<>(MCM::MCMTable::getExpressionDurationMin(), MCM::MCMTable::getExpressionDurationMax())(Constants::RNG);
    }

    void ThreadActor::updateOverrideExpression() {
        int mask = 0;
        if (overrideExpression) {
            mask = overrideExpression->typeMask;
        }
        if (overrideExpressions) {
            overrideExpression = VectorUtil::randomElement(overrideExpressions)->getGenderExpression(isFemale);
            mask &= ~overrideExpression->typeMask;
            applyExpression(overrideExpression, overrideExpression->typeMask, 5);
            overwriteExpressionCooldown = std::uniform_int_distribution<>(MCM::MCMTable::getExpressionDurationMin(), MCM::MCMTable::getExpressionDurationMax())(Constants::RNG);
        } else {
            overrideExpression = nullptr;
            overwriteExpressionCooldown = -1;
        }
        
        wakeExpressions(mask);
    }

    void ThreadActor::setEventExpression(Trait::FacialExpression* expression) {
        int mask = 0;
        if (eventExpression) {
            mask = eventExpression->typeMask;
        }
        eventExpression = expression->getGenderExpression(isFemale);
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

        auto faceData = actor->GetFaceGenAnimationData();
        if (!faceData) {
            return;
        }

        // expression
        if ((mask & Trait::ExpressionType::EXPRESSION) == Trait::ExpressionType::EXPRESSION) {
            if (auto value = expression->expression.calculate(speed, excitement)) {
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
                    goal = iter->second.calculate(speed, excitement);
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
                    goal = iter->second.calculate(speed, excitement);
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
                    goal = iter->second.calculate(speed, excitement);
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
                    goal = iter->second.calculate(speed, excitement);
                    delay = iter->second.randomizeDelay();
                }
                if (current == goal) {
                    phonemeUpdaters.erase(i);
                } else {
                    phonemeUpdaters[i] = {.delay = delay, .current = current, .goal = goal, .speed = updateSpeed};
                }
            }

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
        auto faceData = actor->GetFaceGenAnimationData();

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
                goal = iter->second.calculate(speed, excitement);
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
        for (auto& [type, object] : equipObjects) {
            object.unequip(actor);
            object.removeItems(actor);
        }

        if (MCM::MCMTable::animateRedress() && !isPlayer) {
            const auto skyrimVM = RE::SkyrimVM::GetSingleton();
            auto vm = skyrimVM ? skyrimVM->impl : nullptr;
            if (vm) {
                RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                std::vector<RE::TESForm*> weapons = {rightHand, leftHand, ammo};
                auto args = RE::MakeFunctionArguments(std::move(actor), std::move(isFemale), std::move(undressedItems), std::move(weapons));
                vm->DispatchStaticCall("OUndress", "AnimateRedress", args, callback);
            }
        } else {
            if (MCM::MCMTable::usePapyrusUndressing()) {
                // this object will be destroyed before papyrus redressing is done
                // so for this case we need to invoke Redress without a callback here
                const auto skyrimVM = RE::SkyrimVM::GetSingleton();
                auto vm = skyrimVM ? skyrimVM->impl : nullptr;
                if (vm) {
                    RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
                    auto args = RE::MakeFunctionArguments(std::move(threadId), std::move(actor), std::move(undressedItems));
                    vm->DispatchStaticCall("OUndress", "Redress", args, callback);
                }
            } else {
                redress();
            }
            addWeapons();
        }
        applyHeelOffset(false);
        ActorUtil::setScale(actor, scaleBefore);
        
        freeActor(actor, false);

        // no need to do this in ActorUtil::free since facedata isn't written into the savefile anyways
        auto faceData = actor->GetFaceGenAnimationData();
        if (faceData) {
            faceData->ClearExpressionOverride();
            faceData->Reset(0.0, true, true, true, false);
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
        float currentScale = threadActor->actor->GetReferenceRuntimeData().refScale / 100.0f;
        ActorUtil::setScale(threadActor->actor, currentScale / height);

        if (MCM::MCMTable::groupAlignmentByHeight()) {
            ThreadManager::GetSingleton()->GetThread(threadActor->threadId)->alignActors();
        }
    }

    Serialization::OldThreadActor ThreadActor::serialize() {
        Serialization::OldThreadActor oldThreadActor;

        oldThreadActor.actor = actor;

        for (auto& [type, object] : equipObjects) {
            for (RE::TESObjectARMO* item : object.toRemove) {
                oldThreadActor.equipObjects.push_back(item);
            }
        }

        return oldThreadActor;
    }
}