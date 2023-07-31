#pragma once

#include "Sound/ActionSound/SoundType.h"

namespace Graph {
	struct ActionActor{
	public:
        float stimulation = 0.0;
        float maxStimulation = 100.0;
        bool fullStrip = false;
        bool moan = false;
        bool talk = false;
        bool muffled = false;
        std::string expressionOverride = "";
        uint32_t requirements = 0;
		uint32_t strippingMask = 0;
		std::unordered_map<std::string, int> ints;
        std::unordered_map<std::string, std::vector<int>> intLists;
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, std::vector<float>> floatLists;
        std::unordered_map<std::string, std::string> strings;
        std::unordered_map<std::string, std::vector<std::string>> stringLists;
	};

	struct ActionAttributes {
	public:
		ActionActor actor;
		ActionActor target;
		ActionActor performer;
        std::vector<Sound::SoundType*> sounds;
        std::vector<std::string> tags;

		bool hasTag(std::string tag);
    };

    struct Action {
    public:
        std::string type = "";
        ActionAttributes* attributes = nullptr;
        int actor = -1;
        int target = -1;
        int performer = -1;

        bool doFullStrip(int position);
        uint32_t getStrippingMask(int position);
    };
}