#pragma once

#include "Event.h"
#include "Node.h"
#include "Requirement.h"

#include "Furniture/Furniture.h"
#include "Trait/Condition.h"

namespace Graph{
    class GraphTable {
    public:
        static void addNode(Node* node);
        static Node* getNodeById(std::string id);
        static Node* getNodeByAnimation(std::string anim);

        static bool hasNodes(Furniture::FurnitureType furnitureType, int actorCount);
        static Node* getRandomNode(Furniture::FurnitureType furnitureType, std::vector<Trait::ActorConditions> actorConditions, std::function<bool(Node*)> nodeCondition);

        static void SetupActions();
        static ActionAttributes* GetActionAttributesByType(std::string type);

        static void setupEvents();
        static Event* getEvent(std::string eventName);

        static Requirement getRequirement(std::string string);
    private:

        inline static std::unordered_map<std::string, Node*> nodes;
        inline static std::unordered_map<std::string, Node*> animationNodeTable;
        inline static std::unordered_map<Furniture::FurnitureType, std::unordered_map<int, std::vector<Node*>*>*> nodeList;
        inline static std::unordered_map<std::string, ActionAttributes> actions;
        inline static std::unordered_map<std::string, Event> events;

        inline static std::unordered_map<std::string, Requirement> requirements = {
            {"anus", Requirement::ANUS},
            {"breast", Requirement::BREAST},
            {"foot", Requirement::FOOT},
            {"hand", Requirement::HAND},
            {"mouth", Requirement::MOUTH},
            {"nipple", Requirement::NIPPLE},
            {"penis", Requirement::PENIS},
            {"testicles", Requirement::TESTICLES},
            {"vagina", Requirement::VAGINA}
        };
    };
}