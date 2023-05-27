#pragma once

#include "Core/Singleton.h"
#include "UIInterop.h"

namespace UI {
    enum MenuType { kSceneMenu, kAlignMenu };
    class UIState : public OStim::ISingleton<UIState> {
    public:
        void HandleControl(Controls control);
        void SwitchActiveMenu(MenuType type);
        void hideAllMenues();
        inline MenuType GetActiveMenu() { return activeMenu; }

        inline void setGlpyh(int glyph) { this->glyph = glyph; }
    private:
        // TODO remove this again when we no longer need OSA
        int glyph = 0;
        MenuType activeMenu = MenuType::kSceneMenu;
    };
}  // namespace UI