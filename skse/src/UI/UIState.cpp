#include "UI/UIState.h"
#include "UI/Align/AlignMenu.h"
#include "UI/Scene/SceneMenu.h"
#include "Core/ThreadManager.h"

namespace UI {
    void UIState::HandleControl(Controls control, int64_t glyph) {
        if (!OStim::ThreadManager::GetSingleton()->AnySceneRunning()) {
            return;
        }
        switch (activeMenu) {
        case MenuType::kSceneMenu: {
                /*auto menu = GetHud();
                if (menu) {
                    auto ui = GetOSAControlUIRoot(menu, glyph);
                    auto direction = GetControlString(control);
                    if (direction != "") {
                        ui.Invoke(direction.c_str());
                    }
                }*/
            UI::Scene::SceneMenu::Handle(control);
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

    void UIState::loop() {
        /*refreshUIPositionCooldown -= Constants::LOOP_TIME_MILLISECONDS;
        if (refreshUIPositionCooldown <= 0) {
            UI::Settings::LoadSettings();
            UI::Align::AlignMenu::ApplyPositions();
            UI::Scene::SceneMenu::ApplyPositions();
            refreshUIPositionCooldown = UI_UPDATE_LOOP_TIME;
        }*/
    }
    
}  // namespace UI