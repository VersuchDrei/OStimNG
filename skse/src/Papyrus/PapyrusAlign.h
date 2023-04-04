#pragma once

#include "Alignment/Alignments.h"
#include "Core/ThreadManager.h"
#include "UI/Align/AlignMenu.h"
#include "UI/UIState.h"

namespace PapyrusAlign {
    using VM = RE::BSScript::IVirtualMachine;

    void ToggleMenu(RE::StaticFunctionTag*) {
        auto thread = OStim::ThreadManager::GetSingleton();
        auto uiState = UI::UIState::GetSingleton();

        if (!thread->AnySceneRunning()) {
            UI::Align::AlignMenu::Hide();
            return;
        }
        
        if (uiState->GetActiveMenu() != UI::MenuType::kAlignMenu) {
            uiState->SwitchActiveMenu(UI::MenuType::kAlignMenu);
        }
        else {
            uiState->SwitchActiveMenu(UI::MenuType::kSceneMenu);
        }
    }

    bool Bind(VM* a_vm) {
        const auto obj = "OAlign"sv;

        BIND(ToggleMenu);

        return true;
    }
}