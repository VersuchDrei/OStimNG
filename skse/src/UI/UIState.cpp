#include "UI/UIState.h"
#include "UI/Align/AlignMenu.h"
#include "UI/Scene/SceneMenu.h"
#include "UI/Search/SearchMenu.h"
#include "Core/ThreadManager.h"

namespace UI {
    void UIState::HandleControl(Controls control) {
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
        
        UI::Align::AlignMenu::Hide();
        UI::Search::SearchMenu::Hide();

        if (menu == MenuType::kAlignMenu) {
            UI::Align::AlignMenu::Show();
        } else if(menu == MenuType::kSearchMenu) {
            UI::Search::SearchMenu::Show();
        }
    }
    void UIState::CloseActiveMenu() {
        if (activeMenu != kSceneMenu) {
            ToggleActiveMenu(activeMenu);
        }
    }

    void UIState::ToggleActiveMenu(MenuType menu) {
        if (menu == kSceneMenu)
            return;
        if (activeMenu == kSceneMenu) {
            SwitchActiveMenu(menu);
            return;
        }
        if (activeMenu == menu) {
            SwitchActiveMenu(kSceneMenu);
            return;
        }
    }

    void UIState::SetThread(OStim::Thread* thread) {
        currentThread = thread;
        currentNode = thread->getCurrentNode();
        UI::Align::AlignMenu::ThreadChanged();
        UI::Scene::SceneMenu::UpdateMenuData();
    }

    void UIState::NodeChanged(OStim::Thread* thread, Graph::Node* node) {
        if (!thread || !node) return;
        if (!currentThread->isSameThread(thread)) return;

        currentNode = node;
        UI::Align::AlignMenu::NodeChanged();
        UI::Scene::SceneMenu::UpdateMenuData();
    }
    void UIState::HandleThreadRemoved(OStim::Thread* thread) {
        if (currentThread == thread) {
            currentThread = nullptr;
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