#pragma once
#include <UI/UIInterop.h>
#include <UI/Settings.h>
#include <UI/OStimMenu.h>
#include <Core/Thread.h>
#include <Core/Singleton.h>

namespace UI::Scene {
	class SceneMenu : public UI::OStimMenu {
	public:
		using Super = UI::OStimMenu;

	public:
		static constexpr const char* MENU_NAME = "OstimSceneMenu";
		static constexpr std::int8_t SORT_PRIORITY{ 3 };

		SceneMenu();
		inline static SceneMenu* GetMenu() {
			return static_cast<SceneMenu*>(RE::UI::GetSingleton()->GetMenu(MENU_NAME).get());
		}
		static void Register() { OStimMenu::Register(MENU_NAME, Creator); }
		static RE::stl::owner<RE::IMenu*> Creator() { return new SceneMenu(); }

		void Show();		
		
		void Handle(UI::Controls control);
		void ApplyPositions();

		void ChangeAnimation(std::string nodeId);
		void UpdateMenuData();
		void HideSpeed();
		void UpdateSpeed();

		void SpeedUp();
		void SpeedDown();
			
		struct OptionData{
			std::string nodeId;
			std::string title;
			std::string imagePath;
			std::string border;
			std::string description;
			void loadValues(RE::GFxValue& option) {				
				option.SetMember("NodeID", RE::GFxValue{ nodeId.c_str() });
				option.SetMember("Title", RE::GFxValue{ title.c_str() });
				option.SetMember("ImagePath", RE::GFxValue{ imagePath.c_str() });
				option.SetMember("Border", RE::GFxValue{ border.c_str() });
				option.SetMember("Description", RE::GFxValue{ description.c_str() });
			}
		};

		struct MenuData {
			std::vector<OptionData> options;
			void loadValues(RE::GPtr<RE::GFxMovieView> view, RE::GFxValue& menu) {				
				for (auto& option : options) {
					RE::GFxValue optionValue;
					view->CreateObject(&optionValue);
					option.loadValues(optionValue);
					menu.PushBack(optionValue);
				}				
			}
		};

		 void BuildMenuData(MenuData& menudata);
		 void UpdateInfoBox();
		 void SendControl(int32_t control);
	private:

	};

	class doSendTransitionRequest : public RE::GFxFunctionHandler {
	public:
		void Call(Params& args) override {
			UI::Scene::SceneMenu::GetMenu()->ChangeAnimation(args.args[0].GetString());
		}
	};
}