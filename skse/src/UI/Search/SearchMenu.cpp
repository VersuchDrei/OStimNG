#include "SearchMenu.h"
#include "UI/UIState.h"
#include "Graph/GraphTable.h"

namespace UI::Search {

    inline RE::GFxValue GetRoot(RE::GPtr<RE::GFxMovieView> uiMovie) {
        assert(uiMovie);
        RE::GFxValue root;        

        uiMovie->GetVariable(&root, "_root");
        return root;
    }
    inline RE::GFxValue GetRoot() {
        RE::GPtr<RE::IMenu> searchMenu = RE::UI::GetSingleton()->GetMenu(SearchMenu::MENU_NAME);
        GetRoot(searchMenu->uiMovie);
    }

    inline RE::GFxValue GetSearchBox(RE::GPtr<RE::GFxMovieView> uiMovie) {
        auto root = GetRoot(uiMovie);
        
        RE::GFxValue searchBoxContainer;
        root.GetMember("searchMCContainer", &searchBoxContainer);
        RE::GFxValue searchBox;
        searchBoxContainer.GetMember("searchMC", &searchBox);
        return searchBox;
    }

    inline RE::GFxValue GetSearchBox() {
        RE::GPtr<RE::IMenu> searchMenu = RE::UI::GetSingleton()->GetMenu(SearchMenu::MENU_NAME);
        return GetSearchBox(searchMenu->uiMovie);
    }

	SearchMenu::SearchMenu() : Super(MENU_NAME) {

        inputContext = Context::kMenuMode;

        menuFlags.set(
            RE::UI_MENU_FLAGS::kAlwaysOpen,
            RE::UI_MENU_FLAGS::kRequiresUpdate,
            RE::UI_MENU_FLAGS::kModal,
            RE::UI_MENU_FLAGS::kUsesCursor,
            RE::UI_MENU_FLAGS::kPausesGame,
            RE::UI_MENU_FLAGS::kAdvancesUnderPauseMenu,
            RE::UI_MENU_FLAGS::kRendersUnderPauseMenu,
            RE::UI_MENU_FLAGS::kUsesBlurredBackground
            );

        if (uiMovie) {
            uiMovie->SetMouseCursorCount(1);  // enable input            
        }
        Locker locker(_lock);
        auto optionBoxes = GetSearchBox(uiMovie);

        RE::GFxFunctionHandler* fn = new UI::doHideMenuRequest;
        RE::GFxValue doHideFn;
        _view->CreateFunction(&doHideFn, fn);
        optionBoxes.SetMember("doHideMenuRequest", doHideFn);

        RE::GFxFunctionHandler* fn2 = new doSearchFunction;
        RE::GFxValue doSearchFn;
        _view->CreateFunction(&doSearchFn, fn2);
        optionBoxes.SetMember("doSearch", doSearchFn);

        RE::GFxFunctionHandler* fn3 = new doSelectOptionFunction;
        RE::GFxValue doSelectFn;
        _view->CreateFunction(&doSelectFn, fn3);
        optionBoxes.SetMember("doSelectOption", doSelectFn);
	}

    void SearchMenu::Show() {
        OStimMenu::Show(); 
        Locker locker(_lock);
        auto box = GetSearchBox();
        auto controlMap = RE::ControlMap::GetSingleton();
        controlMap->AllowTextInput(true);
        const RE::GFxValue arg{ true };
        box.Invoke("SetIsOpen", nullptr, &arg, 1);
        ApplyPositions();
    }

    void SearchMenu::Hide() {
        OStimMenu::Hide();
        Locker locker(_lock);
        auto box = GetSearchBox();
        auto controlMap = RE::ControlMap::GetSingleton();
        controlMap->AllowTextInput(false);
        const RE::GFxValue arg{ false };
        box.Invoke("SetIsOpen", nullptr, &arg, 1);
        _isOpen = false;        
    }

    void SearchMenu::ApplyPositions() {
        RE::GPtr<RE::IMenu> searchMenu = RE::UI::GetSingleton()->GetMenu(SearchMenu::MENU_NAME);
        auto root = GetRoot(searchMenu->uiMovie);
        if (!root.IsObject())
            return;

        auto controlPositions = &UI::Settings::positionSettings.ScenePositions.ControlPosition;
        const RE::GFxValue controlX = RE::GFxValue{ controlPositions->xPos };
        const RE::GFxValue controlY = RE::GFxValue{ controlPositions->yPos };
        const RE::GFxValue controlXScale = RE::GFxValue{ controlPositions->xScale };
        const RE::GFxValue controlYScale = RE::GFxValue{ controlPositions->yScale };
        RE::GFxValue controlPosArray[4]{ controlX, controlY, controlXScale, controlYScale };

        RE::GFxValue alignmentInfo;
        root.GetMember("searchMCContainer", &alignmentInfo);
        alignmentInfo.Invoke("setPosition", nullptr, controlPosArray, 4);
    }

    void SearchMenu::AssignData(std::vector<std::string>& data) {
        Locker locker(_lock);
        auto box = GetSearchBox();
        RE::GFxValue arg;
        _view->CreateArray(&arg);
        for (auto& item : data) {
            RE::GFxValue entry;
            _view->CreateObject(&entry);
            entry.SetMember("label", RE::GFxValue{ item.c_str() });
            arg.PushBack(entry);
        }
        box.Invoke("AssignData", nullptr, &arg, 1);
    }

    void SearchMenu::Search(std::string value) {
        std::vector<Graph::Node*> results;
        Graph::GraphTable::findNodesById(value , results);
        std::vector<std::string> data;
        auto state = UI::UIState::GetSingleton();
        for (int i = 0; i < results.size(); i++) {
            if(results[i]->actors.size() == state->currentThread->getActorCount() 
                && results[i]->furnitureType == state->currentNode->furnitureType 
                && !results[i]->isTransition)
            data.push_back(results[i]->scene_id);
        }
        AssignData(data);
    }

    void SearchMenu::SelectOption(std::string val) {
        auto node = Graph::GraphTable::getNodeById(val);

        SKSE::GetTaskInterface()->AddTask([node]() {
            UI::UIState::GetSingleton()->currentThread->ChangeNode(node);
        });
    }
}