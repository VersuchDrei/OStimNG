#pragma once

#include "Core/Singleton.h"
#include "UIInterop.h"
#include "Align/AlignMenu.h"
#include "Scene/SceneMenu.h"
#include "Search/SearchMenu.h"

namespace UI {
    inline static void ShowMenus() {
        UI::Align::AlignMenu::Show();
        UI::Scene::SceneMenu::Show();        
    }

    inline static void RegisterMenus() {
        UI::Align::AlignMenu::Register();
        UI::Scene::SceneMenu::Register();
        UI::Search::SearchMenu::Register();
    }

    inline static void HideMenus() {
        UI::Align::AlignMenu::Hide();
        UI::Scene::SceneMenu::Hide();
        UI::Search::SearchMenu::Hide();
    }

    inline static void PostGameLoad() {
        UI::Scene::SceneMenu::PostRegister();      
        UI::Search::SearchMenu::PostRegister();
    }

    enum MenuType { kSceneMenu, kAlignMenu, kSearchMenu };
    class UIState : public OStim::ISingleton<UIState> {
    public:
        void HandleControl(Controls control, int64_t glyph);
        void CloseActiveMenu();
        void SwitchActiveMenu(MenuType type);
        void ToggleActiveMenu(MenuType type);
        inline MenuType GetActiveMenu() { return activeMenu; }
        void loop();
        void SetThread(OStim::Thread* thread);
        void NodeChanged(OStim::Thread* thread, Graph::Node* node);
        void HandleThreadRemoved(OStim::Thread* thread);


        OStim::Thread* currentThread;
        Graph::Node* currentNode;
    private:
        MenuType activeMenu = MenuType::kSceneMenu;
        const short UI_UPDATE_LOOP_TIME = 10000;
        short refreshUIPositionCooldown = UI_UPDATE_LOOP_TIME;
    };

    class doHideMenuRequest : public RE::GFxFunctionHandler {
    public:
        void Call(Params& args) override {
            auto uiState = UI::UIState::GetSingleton();
            uiState->CloseActiveMenu();
        }
    };
}  // namespace UI