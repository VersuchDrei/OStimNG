#pragma once

#include "Furniture/Furniture.h"
#include "Trait/Condition.h"
#include "Action.h"
#include "Trait/FacialExpression.h"

namespace Graph {
    struct Speed {
    public:
        std::string animation;
        float playbackSpeed = 1.0;
    };

    struct Actor {
    public:
        uint32_t requirements;
        int penisAngle;
        float scale = 1.0;
        float scaleHeight = 120.748;
        bool feetOnGround;
        int expressionAction = -1;
        std::unordered_map<int, Trait::FaceModifier> eyeballModifierOverride;
        std::vector<std::string> tags;
        Trait::ActorConditions conditions;
        std::unordered_map<std::string, std::string> autotransitions;
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
        bool isHub = false;
        bool isAggresive = false;
        bool hasIdleSpeed = false;
        bool noRandomSelection = false;
        Furniture::FurnitureType furnitureType = Furniture::FurnitureType::NONE;
        std::vector<std::string> tags;
        std::vector<Actor*> actors;
        std::vector<Action*> actions;

        // maybe remove this in a later iteration?
        std::string sourceModule;
        std::string animClass;

        void mergeActionRequirementsIntoActors();

        bool fulfilledBy(std::vector<Trait::ActorConditions> conditions);

        std::vector<Trait::FacialExpression*>* getFacialExpressions(int position);
        std::vector<Trait::FacialExpression*>* getOverrideExpressions(int position);

        uint32_t getStrippingMask(int position);
        bool doFullStrip(int position);

        std::string getAutoTransitionForActor(int position, std::string type);

        bool hasActorTag(int position, std::string tag);
        bool hasAnyActorTag(int position, std::vector<std::string> tags);
        bool hasAllActorTags(int position, std::vector<std::string> tags);
        bool hasOnlyListedActorTags(int position, std::vector<std::string> tags);

        int findAction(std::function<bool(Action*)> condition);
        std::vector<int> findActions(std::function<bool(Action*)> condition);

        bool hasActionTag(std::string tag);

        int findAction(std::string type);
        int findAnyAction(std::vector<std::string> types);
        int findActionForActor(int position, std::string type);
        int findAnyActionForActor(int position, std::vector<std::string> types);
        int findActionForTarget(int position, std::string type);
        int findAnyActionForTarget(int position, std::vector<std::string> types);
        int findActionForActorAndTarget(int actorPosition, int targetPosition, std::string type);
        int findAnyActionForActorAndTarget(int actorPosition, int targetPosition, std::vector<std::string> types);
    };
}