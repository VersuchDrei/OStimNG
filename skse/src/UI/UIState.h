#pragma once

#include "Core/Singleton.h"
#include "UIInterop.h"
#include "Align/AlignMenu.h"
#include "Scene/SceneMenu.h"

namespace UI {
    inline static void ShowMenus() {
        UI::Align::AlignMenu::Show();
        UI::Scene::SceneMenu::Show();
    }

    inline static void RegisterMenus() {
        UI::Align::AlignMenu::Register();
        UI::Scene::SceneMenu::Register();
    }
    enum MenuType { kSceneMenu, kAlignMenu };
    class UIState : public OStim::ISingleton<UIState> {
    public:
        void HandleControl(Controls control, int64_t glyph);
        void SwitchActiveMenu(MenuType type);
        void hideAllMenues();
        inline MenuType GetActiveMenu() { return activeMenu; }
    private:
        MenuType activeMenu = MenuType::kSceneMenu;
    };
}  // namespace UI