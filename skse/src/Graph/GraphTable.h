#pragma once

#include "Event.h"
#include "Node.h"
#include "Requirement.h"
#include "Sequence.h"

#include "Furniture/FurnitureType.h"
#include "Trait/Condition.h"

namespace Graph{
    class GraphTable {
    public:
        static Requirement getRequirement(std::string string);
    private:
        inline static std::unordered_map<std::string, Requirement> requirements = {
            {"anus", Requirement::ANUS},
            {"breast", Requirement::BREAST},
            {"foot", Requirement::FOOT},
            {"hand", Requirement::HAND},
            {"mouth", Requirement::MOUTH},
            {"nipple", Requirement::NIPPLE},
            {"penis", Requirement::PENIS},
            {"testicles", Requirement::TESTICLES},
            {"vagina", Requirement::VAGINA},
            {"vampire", Requirement::VAMPIRE}
        };

#pragma region actions
    public:
        static void SetupActions();
        static ActionAttributes* GetActionAttributesByType(std::string type);

    private:
        inline static std::unordered_map<std::string, ActionAttributes> actions;
#pragma endregion

#pragma region events
    public:
        static void setupEvents();
        static Event* getEvent(std::string eventName);

    private:
        inline static std::unordered_map<std::string, Event> events;
#pragma endregion

#pragma region nodes
    public:
        static void setupNodes();
        static void addNode(Node* node);
        static void addNavigations(std::vector<RawNavigation> navigations);
        static Node* getNodeById(std::string id);
        static void searchNodesByName(std::string& name, std::vector<Node*>& results);
        static Node* getNodeByAnimation(std::string anim);

        static bool hasNodes(Furniture::FurnitureType* furnitureType, int actorCount);
        static Node* getRandomNode(Furniture::FurnitureType* furnitureType, std::vector<Trait::ActorCondition> actorConditions, std::function<bool(Node*)> nodeCondition);

    private:
        inline static std::unordered_map<std::string, Node*> nodes;
        inline static std::unordered_map<std::string, Node*> animationNodeTable;
        inline static std::unordered_map<Furniture::FurnitureType*, std::unordered_map<int, std::vector<Node*>*>*> nodeList;
#pragma endregion

#pragma region sequences
    public:
        static void setupSequences();
        static Sequence* getSequenceByID(std::string id);
        static Sequence* getRandomSequence(Furniture::FurnitureType* furnitureType, std::vector<Trait::ActorCondition> actorConditions, std::function<bool(Sequence*)> sequenceCondition);

    private:
        inline static std::unordered_map<std::string, Sequence> sequences;
#pragma endregion
    };
}