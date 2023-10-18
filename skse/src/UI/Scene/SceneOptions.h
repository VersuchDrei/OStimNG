#include "Datatypes.h"

#include "Core/Singleton.h"
#include <Graph/Option.h>

namespace UI::Scene{
	enum HandleResult{
		kExit,
		kHandled
	};

	class SceneOptions : public OStim::ISingleton<SceneOptions> {
	public:
		HandleResult Handle(int i);
		void ExecuteOption(Graph::OptionNode* node);
		void BuildMenuData(MenuData& menuData);
	private:
		Graph::MenuNode* selectedNode;
	};
}