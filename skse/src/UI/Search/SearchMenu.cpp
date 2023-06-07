#include "SearchMenu.h"

namespace UI::Search {
	SearchMenu::SearchMenu() : Super() {
        auto scaleformManager = RE::BSScaleformManager::GetSingleton();

        inputContext = Context::kMenuMode;

        auto menu = static_cast<Super*>(this);
        menu->depthPriority = 0;

        menuFlags.set(
            RE::UI_MENU_FLAGS::kAlwaysOpen,
            RE::UI_MENU_FLAGS::kRequiresUpdate,
            RE::UI_MENU_FLAGS::kAllowSaving,
            RE::UI_MENU_FLAGS::kUsesCursor);

        //if (uiMovie) {
        //    uiMovie->SetMouseCursorCount(0);  // disable input            
        //}

        scaleformManager->LoadMovieEx(this, MENU_PATH, [](RE::GFxMovieDef* a_def) -> void {
            a_def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<Logger>().get());
        });

        view = menu->uiMovie;
	}

    void SearchMenu::Register() {
        auto ui = RE::UI::GetSingleton();
        if (ui) {
            ui->Register(MENU_NAME, Creator);
            logger::info("Registered {}", MENU_NAME);

            RE::GPtr<RE::IMenu> alignMenu = RE::UI::GetSingleton()->GetMenu(MENU_NAME);

            auto msgQ = RE::UIMessageQueue::GetSingleton();
            if (msgQ) {
                msgQ->AddMessage(SearchMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
            }
        }
    }

    void SearchMenu::Show() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(SearchMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
        }
    }

    void SearchMenu::Hide() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(SearchMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        }
    }

    void SearchMenu::AdvanceMovie(float a_interval, std::uint32_t a_currentTime) {
        RE::IMenu::AdvanceMovie(a_interval, a_currentTime);
    }
}