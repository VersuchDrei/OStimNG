#pragma once

namespace Graph {

	struct OptionData {
		std::string name;
		std::string description;
		std::string icon;
		std::string border = "ffffff";
	};

	struct MenuNode : OptionData {
		std::vector<MenuNode> subNodes;
		std::string parentName;
		MenuNode* parent = nullptr;
	};
	struct OptionStateGroupData :OptionData {
		std::string nextState;
	};

	struct OptionNode :MenuNode {
		bool toggle;
		std::unordered_map<std::string, OptionStateGroupData> states;
		std::string script;
		std::string function;
		std::string repeat;
	};

}