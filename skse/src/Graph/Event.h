#pragma once

#include "GameAPI/GameFaction.h"
#include "GameAPI/GameSound.h"

namespace Graph {
    struct EventActor {
        float stimulation = 0.0;
        float maxStimulation = 100.0;
        GameAPI::GameFaction stimulationFaction;
        GameAPI::GameFaction maxStimulationFaction;
        int reactionDelay = 0;
        std::unordered_map<std::string, int> ints;
        std::unordered_map<std::string, std::vector<int>> intLists;
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, std::vector<float>> floatLists;
        std::unordered_map<std::string, std::string> strings;
        std::unordered_map<std::string, std::vector<std::string>> stringLists;
    };

    struct Event {
    public:
        std::string id;
        Event* supertype = nullptr;
        EventActor actor;
        EventActor target;
        EventActor performer;
        GameAPI::GameSound sound;
        float cameraShakeDuration = 0.0;
        float cameraShakeStrength = 0.0;
        float controllerRumbleDuration = 0.0;
        float controllerRumbleStrength = 0.0;
        std::vector<std::string> tags;

        GameAPI::GameSound getSound();
        float getCameraShakeDuration();
        float getCameraShakeStrength();
        float getControllerRumbleDuration();
        float getControllerRumbleStrength();

        float getActorStimulation(GameAPI::GameActor actor);
        float getActorMaxStimulation(GameAPI::GameActor actor);
        float getTargetStimulation(GameAPI::GameActor actor);
        float getTargetMaxStimulation(GameAPI::GameActor actor);
        float getPerformerStimulation(GameAPI::GameActor actor);
        float getPerformerMaxStimulation(GameAPI::GameActor actor);

        float getActorReactionDelay();
        float getTargetReactionDelay();
        float getPerformerReactionDelay();

        bool isChildOf(Event* other);
    };
}