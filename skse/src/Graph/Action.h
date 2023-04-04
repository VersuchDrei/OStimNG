#pragma once

namespace Graph {
	struct ActionActor{
	public:
        float stimulation = 0.0;
        float maxStimulation = 100.0;
        bool fullStrip = false;
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
        std::vector<std::string> tags;

		bool hasTag(std::string tag);
    };

    struct Action {
    public:
        std::string type;
        ActionAttributes* attributes;
        int actor;
        int target;
        int performer;

        bool doFullStrip(int position);
        uint32_t getStrippingMask(int position);
    };
}