#pragma once

#include "Action.h"
#include "GraphActor.h"
#include "RawNavigation.h"

#include "Furniture/Furniture.h"

namespace Graph {
    struct Node; // this line is necessary for the Navigation struct to work

    struct Speed {
    public:
        std::string animation = "";
        float playbackSpeed = 1.0;
        float displaySpeed = -1;
    };

    struct Navigation {
        std::vector<Node*> nodes;
        std::string description = "";
        std::string icon = "";
        std::string border = "ffffff";
        bool isTransition = false;

        bool fulfilledBy(std::vector<Trait::ActorCondition> conditions);
    };

    struct Node {
    public:
        std::string scene_id;
        std::string lowercase_id;
        uint32_t numActors;
        std::string scene_name;
        std::vector<Speed> speeds;
        uint32_t defaultSpeed = 0;
        bool isTransition = false;
        int animationLengthMs = 0;
        bool isHub = false;
        bool hasIdleSpeed = false;
        bool noRandomSelection = false;
        Furniture::FurnitureType furnitureType = Furniture::FurnitureType::NONE;
        std::unordered_map<std::string, std::string> autoTransitions;
        std::vector<std::string> tags;
        std::vector<GraphActor> actors;
        std::vector<Action> actions;
        std::vector<Navigation> navigations;
        std::string modpack = "";

        void mergeActionsIntoActors();

        bool fulfilledBy(std::vector<Trait::ActorCondition> conditions);

        std::vector<Trait::FacialExpression*>* getFacialExpressions(int position);
        std::vector<Trait::FacialExpression*>* getOverrideExpressions(int position);

        uint32_t getStrippingMask(int position);
        bool doFullStrip(int position);

        std::string getAutoTransitionForActor(int position, std::string type);

        bool hasNodeTag(std::string tag);
        bool hasActorTag(int position, std::string tag);
        bool hasAnyActorTag(int position, std::vector<std::string> tags);
        bool hasAllActorTags(int position, std::vector<std::string> tags);
        bool hasOnlyListedActorTags(int position, std::vector<std::string> tags);
        bool hasActorTagOnAny(std::string tag);

        int findAction(std::function<bool(Action)> condition);
        std::vector<int> findActions(std::function<bool(Action)> condition);

        bool hasActionTag(std::string tag);

        int findAction(std::string type);
        int findAnyAction(std::vector<std::string> types);
        int findActionForActor(int position, std::string type);
        int findAnyActionForActor(int position, std::vector<std::string> types);
        int findActionForTarget(int position, std::string type);
        int findAnyActionForTarget(int position, std::vector<std::string> types);
        int findActionForActorAndTarget(int actorPosition, int targetPosition, std::string type);
        int findAnyActionForActorAndTarget(int actorPosition, int targetPosition, std::vector<std::string> types);

#pragma region navigation
    public:
        Node* getRandomNodeInRange(int distance, std::vector<Trait::ActorCondition> actorConditions, std::function<bool(Node*)> nodeCondition);
        std::vector<Node*> getRoute(int distance, std::vector<Trait::ActorCondition> actorConditions, Node* destination);
#pragma endregion
    };
}