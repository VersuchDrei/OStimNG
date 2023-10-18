#include "SceneOptions.h"
#include <Graph/GraphTable.h>
#include <Util/UIUtil.h>


namespace UI::Scene {

	HandleResult SceneOptions::Handle(int i) {
		if (i == -1) {			
			if (selectedNode == nullptr) {				
				return kExit;
			}
			else if (selectedNode->parent == nullptr) {
				selectedNode = nullptr;
			}
			else {
				selectedNode = selectedNode->parent;
			}
		}
		else if (selectedNode == nullptr) {
			selectedNode = &Graph::GraphTable::getOptions()[i];
		} 
		else if (selectedNode->subNodes[i].subNodes.size() > 0) {
			selectedNode = &selectedNode->subNodes[i];
		}
		else {
			auto ptr = &selectedNode->subNodes[i];
			ExecuteOption((Graph::OptionNode*)ptr);
		}
		return kHandled;
	}

	void SceneOptions::ExecuteOption(Graph::OptionNode* option){
		logger::info("Do the thing {} {}", option->script, option->function);
		return;
		const auto skyrimVM = RE::SkyrimVM::GetSingleton();
		auto vm = skyrimVM ? skyrimVM->impl : nullptr;
		if (vm) {
			RE::BSTSmartPointer<RE::BSScript::IStackCallbackFunctor> callback;
			auto args = RE::MakeFunctionArguments();
			vm->DispatchStaticCall(option->script, option->function, args, callback);
		}
	}
	
	void SceneOptions::BuildMenuData(MenuData& menuData) {
		logger::info("node selected? {}", selectedNode != nullptr);
		if (selectedNode == nullptr) {
			menuData.options.push_back({
				.nodeId = "-1",
				.title = "exit",
				.imagePath = UI::getIconTexture("OStim/symbols/x"),
				.border = "ff0000",
				.description = "Return to Navigation"
				});
			auto rootOptions = Graph::GraphTable::getOptions();
			for (int i = 0; i < rootOptions.size(); i++ ) {
				menuData.options.push_back({
					.nodeId = std::to_string(i),
					.title = rootOptions[i].name,
					.imagePath = rootOptions[i].icon,
					.border = rootOptions[i].border,
					.description = rootOptions[i].description
					});
			}
		}
		else {
			logger::info("{}", selectedNode->name);
			logger::info("{}", selectedNode->subNodes.size());
			menuData.options.push_back({
				.nodeId = "-1",
				.title = "back",
				.imagePath = UI::getIconTexture("OStim/symbols/return"),
				.border = "ff0000",
				.description = "Return to " + (selectedNode->parent != nullptr ? selectedNode->parent->name : "Options")
				});
			for (int i = 0; i < selectedNode->subNodes.size(); i++) {
				menuData.options.push_back({
					.nodeId = std::to_string(i),
					.title = selectedNode->subNodes[i].name,
					.imagePath = selectedNode->subNodes[i].icon,
					.border = selectedNode->subNodes[i].border,
					.description = selectedNode->subNodes[i].description
					});
			}
		}
	}
}