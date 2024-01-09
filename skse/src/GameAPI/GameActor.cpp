#include "GameActor.h"

#include "GameTable.h"
#include "GameUtil.h"

#include "Util/MathUtil.h"

namespace GameAPI {
    std::vector<GameActor> GameActor::convertVector(std::vector<RE::Actor*> actors) {
        std::vector<GameActor> ret;
        for (RE::Actor* actor : actors) {
            ret.push_back(actor);
        }
        return ret;
    }

    void GameActor::update3D() const {
        const auto skyrimVM = RE::SkyrimVM::GetSingleton();
        auto vm = skyrimVM ? skyrimVM->impl : nullptr;
        if (vm) {
            RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
            auto args = RE::MakeFunctionArguments();
            auto handle = skyrimVM->handlePolicy.GetHandleForObject(static_cast<RE::VMTypeID>(form->FORMTYPE), form);
            vm->DispatchMethodCall2(handle, "Actor", "QueueNiNodeUpdate", args, callback);
        }
    }

    void GameActor::lock() const {
        if (form->IsPlayerRef()) {
            if (form->AsActorState()->IsWeaponDrawn()) {
                sheatheWeapon();
            }

            RE::PlayerCharacter::GetSingleton()->SetAIDriven(true);
            RE::PlayerControls::GetSingleton()->activateHandler->disabled = true;
        } else {
            bool stop = false;
            stop |= setRestrained(form, true);
            stop |= setDontMove(form, true);
            if (stop) {
                stopMovement(form);
            }
        }

        RE::Actor* actor = form;
        SKSE::GetTaskInterface()->AddTask([actor]() {
            actor->SetGraphVariableBool("bHumanoidFootIKDisable", true);
            actor->SetGraphVariableBool("bHeadTrackSpine", false);
            actor->SetGraphVariableBool("bHeadTracking", false);
            actor->SetGraphVariableBool("tdmHeadtrackingBehavior", false);
        });
    }

    void GameActor::unlock() const{
        if (form->IsPlayerRef()) {
            RE::PlayerCharacter::GetSingleton()->SetAIDriven(false);
            RE::PlayerControls::GetSingleton()->activateHandler->disabled = false;
        } else {
            bool stop = false;
            stop |= setRestrained(form, false);
            stop |= setDontMove(form, false);
            if (stop) {
                stopMovement(form);
            }
        }

        StopTranslation(nullptr, 0, form);

        RE::Actor* actor = form;
        SKSE::GetTaskInterface()->AddTask([actor]() {
            actor->SetGraphVariableBool("bHumanoidFootIKDisable", false);
            actor->SetGraphVariableBool("bHeadTrackSpine", true);
            actor->SetGraphVariableBool("bHeadTracking", true);
            actor->SetGraphVariableBool("tdmHeadtrackingBehavior", true);
            actor->NotifyAnimationGraph("IdleForceDefaultState");
        });
    }

    void GameActor::playAnimation(std::string animation) const {
        RE::Actor* actor = form;
        SKSE::GetTaskInterface()->AddTask([actor, animation] {
            actor->NotifyAnimationGraph(animation);
        });
    }

    void GameActor::playAnimation(std::string animation, float playbackSpeed) const {
        RE::Actor* actor = form;
        SKSE::GetTaskInterface()->AddTask([actor, animation, playbackSpeed] {
            actor->SetGraphVariableFloat("OStimSpeed", playbackSpeed);
            actor->NotifyAnimationGraph(animation);
        });
    }

    bool GameActor::isSex(GameSex sex) const {
        RE::SEX actorSex = form->GetActorBase()->GetSex();
        switch (sex) {
            case MALE:
                return actorSex == RE::SEX::kMale;
            case FEMALE:
                return actorSex == RE::SEX::kFemale;
            case AGENDER:
                return actorSex == RE::SEX::kNone;
        }
        return false;
    }

    bool GameActor::isHuman() const {
        return !form->IsChild() && form->HasKeyword(GameTable::getNPCKeyword()) && form->GetActorBase()->GetRace() != GameTable::getManakinRace();
    }

    void GameActor::setScale(float scale) const {
        RE::Actor* actor = form;
        SKSE::GetTaskInterface()->AddTask([actor, scale] {
            SetScale(actor, scale);
        });
    }

    void GameActor::setRotation(float rotation) const {
        // set rotation Z doesn't work on NPCs
        // and SetAngle causes weird stuttering on the PC
        if (form->IsPlayerRef()) {
            form->SetRotationZ(rotation);
        } else {
            SetAngle(nullptr, 0, form, 0, 0, MathUtil::toDegrees(rotation));
        }
    }

    void GameActor::setPosition(GamePosition position) const {
        setRotation(position.r);
        SetPosition(form, position.x, position.y, position.z);
    }

    void GameActor::lockAtPosition(float x, float y, float z, float r) const {
        StopTranslation(nullptr, 0, form);
        setRotation(r);
        TranslateTo(nullptr, 0, form, x, y, z, 0, 0, MathUtil::toDegrees(r) + 1, 1000000, 0.0001);
    }

    int GameActor::getRelationshipRank(GameActor other) const {
        return 4 - RE::BGSRelationship::GetRelationship(form->GetActorBase(), other.form->GetActorBase())->level.underlying();
    }

    void GameActor::sheatheWeapon() const {
        const auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
        const auto script = factory ? factory->Create() : nullptr;
        if (script) {
            script->SetCommand("rae WeaponSheathe"sv);
            GameUtil::CompileAndRun(script, form);
            delete script;
        }
    }

    std::vector<GameActor> GameActor::getNearbyActors(float radius, std::function<bool(GameActor)> condition) const {
        std::vector<GameActor> actors;

        GameUtil::forEachReferenceInRange(form->GetPosition(), radius, [&actors, &condition](RE::TESObjectREFR& ref) {
            if (!ref.Is(RE::Actor::FORMTYPE)) {
                return RE::BSContainer::ForEachResult::kContinue;
            }

            GameActor actor = ref.As<RE::Actor>();

            if (!condition(actor)) {
                return RE::BSContainer::ForEachResult::kContinue;
            }

            actors.push_back(actor);

            return RE::BSContainer::ForEachResult::kContinue;
        });

        RE::NiPoint3 center = form->GetPosition();
        std::sort(actors.begin(), actors.end(), [&](GameActor actorA, GameActor actorB) {
            return actorA.form->GetPosition().GetSquaredDistance(center) < actorB.form->GetPosition().GetSquaredDistance(center);
        });

        return actors;
    }
}