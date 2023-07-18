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
            UI::Scene::SceneMenu::GetMenu()->Handle(control);
            } break;
        case MenuType::kAlignMenu: {
                UI::Align::AlignMenu::GetMenu()->Handle(control);
            } break;
        }
    }

    void UIState::SwitchActiveMenu(MenuType menu) {
        activeMenu = menu;
        
        UI::Align::AlignMenu::GetMenu()->Hide();
        UI::Search::SearchMenu::GetMenu()->Hide();

        if (menu == MenuType::kAlignMenu) {
            UI::Align::AlignMenu::GetMenu()->Show();
        } else if(menu == MenuType::kSearchMenu) {
            UI::Search::SearchMenu::GetMenu()->Show();
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
        UI::Align::AlignMenu::GetMenu()->ThreadChanged();
        UI::Scene::SceneMenu::GetMenu()->UpdateMenuData();
    }

    void UIState::NodeChanged(OStim::Thread* thread, Graph::Node* node) {
        if (!thread || !node) return;
        if (!currentThread->isSameThread(thread)) return;
        
        currentNode = node;
        SKSE::GetTaskInterface()->AddTask([node]() {
            UI::Align::AlignMenu::GetMenu()->NodeChanged();
            UI::Scene::SceneMenu::GetMenu()->UpdateMenuData();
            UI::Scene::SceneMenu::GetMenu()->UpdateSpeed();
        });        
    }

    void UIState::SpeedChanged(OStim::Thread* thread, int speed) {
        if (!thread) return;
        if (!currentThread->isSameThread(thread)) return;

        UI::Scene::SceneMenu::GetMenu()->UpdateSpeed();
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