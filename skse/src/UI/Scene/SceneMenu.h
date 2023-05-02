#pragma once
#include <UI/UIInterop.h>
#include <UI/Settings.h>
#include <Core/Thread.h>

namespace UI::Scene {
	class SceneMenu : public RE::IMenu {
	public:
		using Super = RE::IMenu;

	public:
		static constexpr const char* MENU_PATH = "OstimSceneMenu";
		static constexpr const char* MENU_NAME = "OstimSceneMenu";
		static constexpr std::int8_t SORT_PRIORITY{ 3 };

		SceneMenu();

		static void Register();
		static void Show();
		static void Hide();
		static void Update();

		static RE::stl::owner<RE::IMenu*> Creator() { return new SceneMenu(); }

		void AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override;

		
		static void Handle(UI::Controls control);
		static void ApplyPositions();

		static void SetThread(OStim::Thread* thread);
		static void NodeChanged(OStim::Thread* thread, Graph::Node* node);
		
	private:
		class Logger : public RE::GFxLog {
		public:
			void LogMessageVarg(LogMessageType, const char* a_fmt, std::va_list a_argList) override {
				std::string fmt(a_fmt ? a_fmt : "");
				while (!fmt.empty() && fmt.back() == '\n') {
					fmt.pop_back();
				}

				std::va_list args;
				va_copy(args, a_argList);
				std::vector<char> buf(static_cast<std::size_t>(std::vsnprintf(0, 0, fmt.c_str(), a_argList) + 1));
				std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
				va_end(args);

				logger::info("{}"sv, buf.data());
			}
		};
		
		struct OptionData{
			std::string nodeId;
			std::string title;
			std::string imagePath;
			std::string description;
			void loadValues(RE::GFxValue& option) {				
				option.SetMember("NodeID", RE::GFxValue{ nodeId.c_str()});
				option.SetMember("Title", RE::GFxValue{ title.c_str()});
				option.SetMember("ImagePath", RE::GFxValue{ imagePath.c_str()});
				option.SetMember("Description", RE::GFxValue{ description.c_str()});
			}
		};

		struct MenuData {
			std::vector<OptionData> options;
			void loadValues(RE::GFxValue& menu) {				
				for (auto& option : options) {
					RE::GFxValue optionValue;
					view->CreateObject(&optionValue);
					option.loadValues(optionValue);
					menu.PushBack(optionValue);
				}				
			}
		};

		static void UpdateMenuData();
		static MenuData BuildMenuData();
		static void UpdateInfoBox();
		static void SendControl(int32_t control);
	private:
		inline static OStim::Thread* currentThread;
		inline static Graph::Node* currentNode;

		inline static RE::GPtr<RE::GFxMovieView> view;
	};
}