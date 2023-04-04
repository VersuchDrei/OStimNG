#include "UI/UIState.h"
#include "UI/Align/AlignMenu.h"
#include "Core/ThreadManager.h"

namespace UI {
    void UIState::HandleControl(Controls control, int64_t glyph) {
        if (!OStim::ThreadManager::GetSingleton()->AnySceneRunning()) {
            return;
        }
        switch (activeMenu) {
        case MenuType::kSceneMenu: {
                auto menu = GetHud();
                if (menu) {
                    auto ui = GetOSAControlUIRoot(menu, glyph);
                    auto direction = GetControlString(control);
                    if (direction != "") {
                        ui.Invoke(direction.c_str());
                    }
                }
            } break;
        case MenuType::kAlignMenu: {
                UI::Align::AlignMenu::Handle(control);
            } break;
        }
    }

    void UIState::SwitchActiveMenu(MenuType menu) {
        activeMenu = menu;
        if (menu == MenuType::kAlignMenu) {
            UI::Align::AlignMenu::Show();
        } else {
            UI::Align::AlignMenu::Hide();
        }
    }

    void UIState::hideAllMenues() {
        if (activeMenu == MenuType::kAlignMenu) {
            UI::Align::AlignMenu::Hide();
        }
    }
}  // namespace UI