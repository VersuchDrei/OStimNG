#pragma once

#include "Core/Singleton.h"
#include "UIInterop.h"

namespace UI {
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