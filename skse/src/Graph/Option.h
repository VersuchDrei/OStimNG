#pragma once

namespace Graph {

	struct OptionData {
		std::string name;
		std::string description;
		std::string icon;
		std::string border = "ffffff";
	};
	struct OptionNode;
	struct MenuNode : OptionData {
		std::vector<MenuNode> subNodes;
		std::vector<OptionNode> options;
		std::string parentName;
		MenuNode* parent = nullptr;
		std::string repeat;
	};

	struct OptionNode :MenuNode {
		std::unordered_map<std::string, OptionData> states;
		std::string script;
		std::string function;
		std::string stateFaction = "";
	};
}