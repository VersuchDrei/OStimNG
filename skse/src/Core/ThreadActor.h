#pragma once

#include "EquipObjectHandler.h"
#include "ExpressionUpdater.h"

#include "Alignment/ActorKey.h"
#include "GameAPI/GameActor.h"
#include "Graph/Event.h"
#include "Graph/Node.h"
#include "Serial/OldThread.h"
#include "Sound/VoiceSet.h"
#include "Trait/EquipObject.h"

namespace OStim {
    class Thread;

	class ThreadActor {
	public:
        ThreadActor(Thread* thread, int index, GameAPI::GameActor actor);
        void initContinue();

        int index;

		inline GameAPI::GameActor& getActor() { return actor; }

        inline Thread* getThread() { return thread; }

        Alignment::ActorKey getAlignmentKey();

        void undress();
        void undressPartial(uint32_t mask);
        void removeWeapons();
        void redress();
        void redressPartial(uint32_t mask);
        void addWeapons();

        void changeNode(Graph::GraphActor* graphActor, std::vector<Trait::FacialExpression*>* nodeExpressions, std::vector<Trait::FacialExpression*>* overrideExpressions);
        void changeSpeed(int speed);
        void setScaleMult(float scaleMult);
        void setSoSBend(int sosBend);
        void offsetSoSBend(int sosOffset);

        void handleNiNodeUpdate();

        void loop();

        void free();

        inline bool isFemale() { return female; }
        inline bool hasSchlong() { return schlong; }

        inline Graph::GraphActor* getGraphActor() { return graphActor; }

        Serialization::OldThreadActor serialize();

    private:
        class GetRmHeightCallbackFunctor : public RE::BSScript::IStackCallbackFunctor {
        public:
            GetRmHeightCallbackFunctor(ThreadActor* threadActor) : threadActor{threadActor} {}

            void setRmHeight(float height);

            virtual inline void operator()(RE::BSScript::Variable a_result) override {
                if (a_result.IsNoneObject()) {
                    logger::info("result is none");
                } else if (a_result.IsFloat()) {
                    setRmHeight(a_result.GetFloat());
                } else {
                    logger::info("result is not a float");
                }
            }

            virtual inline void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_object){};

        private:
            ThreadActor* threadActor;
        };

        class PapyrusUndressCallbackFunctor : public RE::BSScript::IStackCallbackFunctor {
        public:
            PapyrusUndressCallbackFunctor(ThreadActor* threadActor, bool isRedress) : threadActor{threadActor}, isRedress{isRedress} {}

            virtual inline void operator()(RE::BSScript::Variable a_result) override {
                if (a_result.IsNoneObject()) {
                    logger::info("result is none");
                } else if (a_result.IsNoneArray()) {
                    logger::info("result is none array");
                } else if (a_result.IsObjectArray()) {
                    auto items = a_result.GetArray().get();
                    std::vector<RE::TESObjectARMO*> armors;
                    for (int i = 0; i < items->size(); i++) {
                        RE::BSScript::Variable item = items->operator[](i);
                        if (item.IsObject()) {
                            auto armor = item.Unpack<RE::TESObjectARMO*>();
                            armors.push_back(armor);
                        }
                    }
                    if (isRedress) {
                        threadActor->papyrusRedressCallback(armors);
                    } else {
                        threadActor->papyrusUndressCallback(armors);
                    }
                } else {
                    logger::info("result is not an object array");
                }
            }

            virtual inline void SetObject(const RE::BSTSmartPointer<RE::BSScript::Object>& a_object){};

        private:
            ThreadActor* threadActor;
            bool isRedress;
        };


        Thread* thread;
		GameAPI::GameActor actor;

        float scaleBefore = 1.0;
        GameAPI::GamePosition positionBefore;

        bool isPlayer;
        bool female;
        bool schlong;

        Graph::GraphActor* graphActor = nullptr;
        GameAPI::GameActor primaryPartner = nullptr;
        int speed = 0;
        float scaleMult = 1.0;
        int sosBend = 0;
        int sosOffset = 0;

        bool undressed = false;
        uint32_t undressedMask = 0;
        std::vector<RE::TESObjectARMO*> undressedItems;
        bool weaponsRemoved = false;
        RE::TESForm* rightHand = nullptr;
        RE::TESForm* leftHand = nullptr;
        RE::TESAmmo* ammo = nullptr;

        float rmHeight = 1;
        float heelOffset = 0;
        bool heelOffsetRemoved = false;

        void checkHeelOffset();
        void applyHeelOffset(bool remove);
        void updateHeelOffset();

        void papyrusUndressCallback(std::vector<RE::TESObjectARMO*> items);
        void papyrusRedressCallback(std::vector<RE::TESObjectARMO*> items);

#pragma region animation
    public:
        void playAnimation(Graph::Speed& speed);

    private:
        void scale();
        void bendSchlong();
#pragma endregion

#pragma region climax
    public:
        inline bool getAwaitingClimax() { return awaitingClimax || awaitingClimaxInner; }
        inline bool getStallClimax() { return stallClimax; }
        inline void setStallClimax(bool stallClimax) { this->stallClimax = stallClimax; }
        void orgasm(bool ignoreStall); // handles stalling / starts climax animation
        void climax(); // plays sounds / expressions and fires events
        inline int getTimexClimaxed() { return timesClimaxed; }

    private:
        bool stallClimax = false;
        bool awaitingOrgasm = false; // for when orgasms are stalled
        bool awaitingClimax = false; // for waiting for climax annotations in climax auto transitions
        bool awaitingClimaxInner = false; // for waiting for everyone to stop talking
        int timesClimaxed = 0;

        float timeUntilClimax = -1.0f;

        void loopClimax();
        void climaxInner();

        void setTimeUntilClimax(float time);
#pragma endregion

#pragma region equipobjects
    public:
        bool equipObject(std::string type);
        void unequipObject(std::string type);
        bool isObjectEquipped(std::string type);
        bool setObjectVariant(std::string type, std::string variant, int duration);
        void unsetObjectVariant(std::string type);
        void refreshObject(std::string type);

        inline bool setObjectVariant(std::string type, std::string variant) { return setObjectVariant(type, variant, 0); }

    private:
        std::unordered_map<std::string, EquipObjectHandler> equipObjects;
        std::vector<std::string> phonemeObjects;

        void loopEquipObjects();
#pragma endregion

#pragma region excitement
    public:
        inline float getExcitement() { return excitement; }
        void setExcitement(float value);
        void addExcitement(float value, bool respectMultiplier);
        inline float getMaxExcitement() { return maxExcitement; }
        void setMaxExcitement(float max);
        inline float getBaseExcitementInc() { return baseExcitementInc; }
        inline void setBaseExcitementInc(float inc) { baseExcitementInc = inc; }
        inline float getExcitementMultiplier() { return excitementMultiplier; }
        void setExcitementMultiplier(float multiplier);
        inline void setLoopExcitementInc(float inc) { loopExcitementInc = inc; }

    private:
        float excitement = 0;                  // current excitement
        float baseExcitementInc = 0;           // base excitement per second without speed or MCM modifier
        float excitementMultiplier = 1.0;  // multiplier from MCM
        float loopExcitementInc = 0;           // final excitement inc per loop
        float maxExcitement = 0;
        float loopExcitementDecay = 0;  // excitement decay per loop
        int excitementDecayCooldown = 0;

        void loopExcitement();
        void changeSpeedExcitement();
        void recalculateLoopExcitement();
#pragma endregion

#pragma region expression
    public:
        void setEventExpression(std::string expression);
        void setEventExpression(Trait::FacialExpression* expression);
        void clearEventExpression();
        void playEventExpression(std::string expression);
        void playEventExpression(Trait::FacialExpression* expression);
        bool hasExpressionOverride();
        void setLooking(std::unordered_map<int, Trait::FaceModifier> eyeballOverride);
        void unsetLooking();
        void resetLooking();

    private:
        std::vector<Trait::FacialExpression*>* nodeExpressions = nullptr;
        Trait::GenderExpression* underlyingExpression = nullptr;
        int underlyingExpressionCooldown = 999999;
        std::unordered_map<int, Trait::FaceModifier> eyeballModifierOverride;
        Trait::GenderExpression* eventExpression = nullptr;
        int eventExpressionCooldown = 0;
        std::vector<Trait::FacialExpression*>* overrideExpressions = nullptr;
        Trait::GenderExpression* overrideExpression = nullptr;
        int overwriteExpressionCooldown = 0;
        std::vector<Trait::FacialExpression*>* actorExpressions = nullptr;
        std::unordered_map<int, ExpressionUpdater> modifierUpdaters;
        std::unordered_map<int, ExpressionUpdater> phonemeUpdaters;

        void loopExpression();

        void updateUnderlyingExpression();
        void updateOverrideExpression();
        void wakeExpressions(int mask);
        void applyExpression(Trait::GenderExpression* expression, int mask, int updateSpeed);
        void checkForEyeballOverride();
        void applyEyeballOverride();
#pragma endregion

#pragma region sound
    public:
        void mute();
        void unmute();
        inline bool isMuted() { return actorMuted || muted; }

        void reactToEvent(int timer, Graph::Event* event, GameAPI::GameActor partner, std::function<std::unordered_map<std::string, Sound::ReactionSet>*(Sound::VoiceSet&)> setGetter);
        void reactToClimax(GameAPI::GameActor partner);

    private:
        Sound::VoiceSet voiceSet;
        bool actorMuted = false;
        bool actorMuffled = false;
        bool muted = false;
        bool muffled = false;
        int soundGracePeriod = 0;
        int moanCooldown = -1;
        GameAPI::GameSound* lastMoan = nullptr;

        int eventTimer = 0;
        Sound::ReactionSet* eventReaction = nullptr;
        GameAPI::GameActor eventPartner;

        int dialogueCountdown = 2;
        bool isTalking = false;

        void loopSound();

        bool canMakeSound();
        bool canTalk();

        void startMoanCooldown();
        inline void stopMoanCooldown() { moanCooldown = -1; };

        void playSound(Sound::ReactionSet* reactionSet, GameAPI::GameActor partner, bool ignoreChecks);

        void setDialogueCountdown();

        void playClimaxSound();

        inline bool isMakingSound() { return lastMoan && lastMoan->isPlaying() || actor.isTalking(); }
#pragma endregion
	};	
}