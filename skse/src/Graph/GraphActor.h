#pragma once

#include "GraphActorTag.h"

#include "Action/Action.h"

#include "GameAPI/GamePosition.h"
#include "GameAPI/GameSex.h"
#include "PluginInterface/Graph/NodeActor.h"
#include "Trait/Condition.h"
#include "Trait/FacialExpression.h"

namespace Graph {
    struct GraphActor : public OStim::NodeActor {
    public:
        int sosBend = 0;
        float scale = 1.0;
        float scaleHeight = 120.748;
        bool feetOnGround = false;
        int expressionAction = -1;
        int animationIndex = -1;
        std::string underlyingExpression = "";
        std::string expressionOverride = "";
        bool noStrip = false;
        bool moan = false;
        bool talk = false;
        bool muffled = false;
        GameAPI::GamePosition offset;
        std::vector<GameAPI::GameFaction> factions;
        std::unordered_map<int, Trait::FaceModifier> eyeballModifierOverride;
        std::vector<GraphActorTag> tags;
        Trait::ActorCondition condition;
        std::unordered_map<std::string, std::string> autoTransitions;

        void merge(Action::ActionActor& action);

        bool hasTag(std::string tag);
        bool hasAnyTag(std::vector<std::string> tags);
        bool hasAllTags(std::vector<std::string> tags);
        bool hasOnlyTags(std::vector<std::string> tags);

        virtual bool hasTag(const char* tag) override;
        virtual uint32_t getTagCount() override;
        virtual OStim::NodeActorTag* getTag(uint32_t index) override;
        virtual void forEachTag(OStim::NodeActorTagVisitor* visitor) override;
    };
}