#pragma once

#include "GameActorBone.h"
#include "GameActorValue.h"
#include "GameFaction.h"
#include "GameKeyword.h"
#include "GamePosition.h"
#include "GameRace.h"
#include "GameRecord.h"
#include "GameSex.h"
#include "GameVoice.h"

namespace GameAPI {
    struct GameActor : public GameRecord<RE::Actor> {
    public:
        inline static GameActor getPlayer() { return RE::PlayerCharacter::GetSingleton(); }

        inline GameActor() {}
        inline GameActor(RE::Actor* actor) { form = actor; }

        inline uint32_t getBaseFormID() const { return form->GetActorBase()->formID; }

        void update3D() const;
        inline void updateAI() const { form->EvaluatePackage(); }
        void playAnimation(std::string animation) const;

        bool isSex(GameSex sex) const;
        inline bool isPlayer() const { return form->IsPlayerRef(); }
        inline std::string getName() const { return form->GetDisplayFullName(); }

        inline GameAPI::GameRace getRace() const { return form->GetActorBase()->GetRace(); }
        inline bool isRace(GameRace race) const { return form->GetActorBase()->GetRace() == race.form; }
        bool isHuman() const;
        inline GameAPI::GameVoice getVoice() const { return form->GetActorBase()->voiceType; }
        inline bool hasVoice(GameAPI::GameVoice voice) const { return form->GetActorBase()->voiceType == voice.form; }


        inline float getScale() const { return form->GetReferenceRuntimeData().refScale / 100.0f; }
        inline bool isScale(float scale) const { return static_cast<int>(scale * 100) == form->GetReferenceRuntimeData().refScale; }
        void setScale(float scale) const;
        inline float getHeight() const { return form->GetActorBase()->GetHeight(); }
        inline float getRotation() const { return form->data.angle.z; }
        inline void setRotation(float rotation) const { form->SetRotationZ(rotation); }
        inline GamePosition getPosition() const { return form->GetPosition(); }
        inline void setPosition(GamePosition position) const { SetPosition(form, position.x, position.y, position.z); }

        inline float getActorValue(GameActorValue actorValue) const { return form->AsActorValueOwner()->GetBaseActorValue(actorValue.actorValue); }
        inline void damageActorValue(GameActorValue actorValue, float value) const { form->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, actorValue.actorValue, -value);}

        inline bool hasKeyword(GameKeyword keyword) const { return form->HasKeyword(keyword.keyword); }
        inline bool hasKeyword(std::string keyword) const { return form->HasKeywordString(keyword); }

        inline void addToFaction(GameFaction faction) const { form->AddToFaction(faction.form, 0); }
        inline void removeFromFaction(GameFaction faction) const { RemoveFromFaction(form, faction.form); }
        inline bool isInFaction(GameFaction faction) const { return form->IsInFaction(faction.form); }
        inline bool hasCrimeFaction(GameFaction faction) const { return form->GetCrimeFaction() == faction.form; }
        void setFactionRank(GameFaction faction, int rank) const;
        int getFactionRank(GameFaction faction) const;

        int getRelationshipRank(GameActor other) const;
        inline bool isInCombat() const { return form->IsInCombat(); }
        inline bool isDead() const { return form->IsDead(); }
        inline bool isInSameCell(GameActor other) const {return form->parentCell == other.form->parentCell;}

        inline GameActorBone getBone(std::string bone) const { return form->GetNodeByName(bone); }

        std::vector<GameActor> getNearbyActors(float radius, std::function<bool(GameActor)> condition);

    private:
        inline static void SetScale(RE::Actor* actor, float scale) {
            using func_t = decltype(SetScale);
            REL::Relocation<func_t> func{RELOCATION_ID(19239, 19665)};
            func(actor, scale);
        }

        inline static void RemoveFromFaction(RE::Actor* actor, RE::TESFaction* faction) {
            using func_t = decltype(RemoveFromFaction);
            REL::Relocation<func_t> func{RELOCATION_ID(36680, 37688)};
            func(actor, faction);
        }

        inline static void SetPosition(RE::TESObjectREFR* object, float x, float y, float z) { object->SetPosition(x, y, z); }
    };
}