#pragma once

#include "EquipObject.h"
#include "FacialExpression.h"

namespace Trait {
    class TraitTable {
    public:
        inline static FacialExpression fallbackExpression{};

        inline static std::unordered_map<int, FaceModifier> openMouthPhonemes;
        inline static std::unordered_map<int, FaceModifier> lickingPhonemes;

        static void setup();
        static void setupForms();

        static std::vector<FacialExpression*>* getExpressionsForActionActor(std::string);
        static std::vector<FacialExpression*>* getExpressionsForActionTarget(std::string);
        static std::vector<FacialExpression*>* getExpressionsForEvent(std::string);
        static std::vector<FacialExpression*>* getExpressionsForSet(std::string);

        static void addToExcitementFaction(RE::Actor* actor);
        static void removeFromExcitementFaction(RE::Actor* actor);
        static void setExcitement(RE::Actor* actor, float excitement);
        static float getExcitement(RE::Actor* actor);

        static EquipObject* getRandomEquipObject(std::string type);
        static EquipObject* getEquipObject(RE::Actor* actor, std::string type);

        static std::vector<std::string> getEquipObjectPairs(RE::FormID formID, std::string type);
        static std::string getEquipObjectName(RE::FormID formID, std::string type);
        static void setEquipObjectID(RE::FormID formID, std::string type, std::string id);

        inline static bool areFacialExpressionsBlocked(RE::Actor* actor) {
            return actor->IsInFaction(noFacialExpressionsFaction);
        }

        static bool hasSchlong(RE::Actor* actor);

    private:
        static void parseGender(nlohmann::json json, GenderExpression* genderExpression);
        static FaceModifier parseModifier(nlohmann::json json);
        static void addToTable(std::unordered_map<std::string, std::vector<FacialExpression*>*>* table, std::string key, FacialExpression* expression);
        static std::vector<FacialExpression*>* getExpressionsFromTable(std::unordered_map<std::string, std::vector<FacialExpression*>*> table, std::string key);

        inline static std::unordered_map<std::string, std::vector<FacialExpression*>*> expressionsByActionActors;
        inline static std::unordered_map<std::string, std::vector<FacialExpression*>*> expressionsByActionTargets;
        inline static std::unordered_map<std::string, std::vector<FacialExpression*>*> expressionsByEvents;
        inline static std::unordered_map<std::string, std::vector<FacialExpression*>*> expressionsBySets;

        inline static std::unordered_map <std::string, std::unordered_map<std::string, EquipObject*>> equipObjects;

        inline static RE::TESFaction* excitementFaction;
        inline static RE::TESFaction* noFacialExpressionsFaction;

        inline static RE::TESFaction* SOS_SchlongifiedFaction = nullptr;
    };
}