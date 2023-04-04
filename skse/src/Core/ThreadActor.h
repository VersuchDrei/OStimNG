#pragma once

#include "EquipObjectHandler.h"
#include "ExpressionUpdater.h"

#include "Alignment/ActorKey.h"
#include "Graph/Node.h"
#include "Trait/EquipObject.h"
#include "Serial/OldThread.h"

namespace OStim {
	class ThreadActor {
	public:
        ThreadActor(int threadId, RE::Actor* actor);
        void initContinue();

		float excitement = 0; // current excitement
		float baseExcitementInc = 0; // base excitement per second without speed or MCM modifier
		float baseExcitementMultiplier = 1.0; // multiplier from MCM
        float loopExcitementInc = 0; // final excitement inc per loop
		float maxExcitement = 0;
        float loopExcitementDecay = 0; // excitement decay per loop
        int excitementDecayCooldown = 0;

		inline RE::Actor* getActor() { return actor; }

        Alignment::ActorKey getAlignmentKey();

        void undress();
        void undressPartial(uint32_t mask);
        void removeWeapons();
        void redress();
        void redressPartial(uint32_t mask);
        void addWeapons();

        void changeNode(Graph::Actor* graphActor, std::vector<Trait::FacialExpression*>* nodeExpressions, std::vector<Trait::FacialExpression*>* overrideExpressions);
        void changeSpeed(int speed);
        void setSoSBend(int sosBend);

        void handleNiNodeUpdate();

        void setEventExpression(Trait::FacialExpression* expression);
        void clearEventExpression();
        void setLooking(std::unordered_map<int, Trait::FaceModifier> eyeballOverride);
        void unsetLooking();
        void resetLooking();

        bool equipObject(std::string type);
        void unequipObject(std::string type);
        bool isObjectEquipped(std::string type);
        bool setObjectVariant(std::string type, std::string variant, int duration);
        void unsetObjectVariant(std::string type);

        inline bool setObjectVariant(std::string type, std::string variant) {
            return setObjectVariant(type, variant, 0);
        }

        void loop();

        void free();

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

        int threadId;
		RE::Actor* actor;
        float scaleBefore;
        bool isPlayer;
        bool isFemale;
        bool hasSchlong;

        Graph::Actor* graphActor = nullptr;
        int speed = 0;
        int sosBend = 0;

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

        std::vector<Trait::FacialExpression*>* nodeExpressions = nullptr;
        Trait::GenderExpression* underlyingExpression = nullptr;
        int underlyingExpressionCooldown = 999999;
        std::unordered_map<int, Trait::FaceModifier> eyeballModifierOverride;
        Trait::GenderExpression* eventExpression = nullptr;
        std::vector<Trait::FacialExpression*>* overrideExpressions = nullptr;
        Trait::GenderExpression* overrideExpression = nullptr;
        int overwriteExpressionCooldown = 0;
        std::unordered_map<int, ExpressionUpdater> modifierUpdaters;
        std::unordered_map<int, ExpressionUpdater> phonemeUpdaters;

        std::unordered_map<std::string, EquipObjectHandler> equipObjects;
        std::vector<std::string> phonemeObjects;

        void scale();
        void bendSchlong();
        void checkHeelOffset();
        void applyHeelOffset(bool remove);
        void updateHeelOffset();

        void updateUnderlyingExpression();
        void updateOverrideExpression();
        void wakeExpressions(int mask);
        void applyExpression(Trait::GenderExpression* expression, int mask, int updateSpeed);
        void checkForEyeballOverride();
        void applyEyeballOverride();

        void papyrusUndressCallback(std::vector<RE::TESObjectARMO*> items);
        void papyrusRedressCallback(std::vector<RE::TESObjectARMO*> items);
	};	
}