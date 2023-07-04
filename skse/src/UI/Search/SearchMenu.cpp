#include "SearchMenu.h"
#include "UI/UIState.h"
#include "Graph/GraphTable.h"

namespace UI::Search {

    inline RE::GFxValue GetRoot() {
        RE::GFxValue root;
        RE::GPtr<RE::IMenu> searchMenu = RE::UI::GetSingleton()->GetMenu(SearchMenu::MENU_NAME);
        assert(searchMenu && searchMenu->uiMovie);

        searchMenu->uiMovie->GetVariable(&root, "_root");
        return root;
    }

    inline RE::GFxValue GetSearchBox() {
        auto root = GetRoot();
        RE::GFxValue searchBoxContainer;
        root.GetMember("searchMCContainer", &searchBoxContainer);
        RE::GFxValue searchBox;
        searchBoxContainer.GetMember("searchMC", &searchBox);
        return searchBox;
    }

	SearchMenu::SearchMenu() : Super() {
        auto scaleformManager = RE::BSScaleformManager::GetSingleton();

        inputContext = Context::kMenuMode;

        auto menu = static_cast<Super*>(this);
        menu->depthPriority = 0;

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

    void SearchMenu::PostRegister() {
        auto optionBoxes = GetSearchBox();

        RE::GFxFunctionHandler* fn = new UI::doHideMenuRequest;
        RE::GFxValue doHideFn;
        view->CreateFunction(&doHideFn, fn);
        optionBoxes.SetMember("doHideMenuRequest", doHideFn);

        RE::GFxFunctionHandler* fn2 = new doSearchFunction;
        RE::GFxValue doSearchFn;
        view->CreateFunction(&doSearchFn, fn2);
        optionBoxes.SetMember("doSearch", doSearchFn);

        RE::GFxFunctionHandler* fn3 = new doSelectOptionFunction;
        RE::GFxValue doSelectFn;
        view->CreateFunction(&doSelectFn, fn3);
        optionBoxes.SetMember("doSelectOption", doSelectFn);
    }

    void SearchMenu::Show() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(SearchMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
            auto box = GetSearchBox();
            auto controlMap = RE::ControlMap::GetSingleton();
            controlMap->AllowTextInput(true);
            const RE::GFxValue arg{ true };
            box.Invoke("SetIsOpen", nullptr, &arg, 1);
            ApplyPositions();
        }
    }

    void SearchMenu::Hide() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(SearchMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
            auto box = GetSearchBox();
            auto controlMap = RE::ControlMap::GetSingleton();
            controlMap->AllowTextInput(false);
            const RE::GFxValue arg{ false };
            box.Invoke("SetIsOpen", nullptr, &arg, 1);
        }
    }

    void SearchMenu::ApplyPositions() {
        auto root = GetRoot();
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

    void SearchMenu::AdvanceMovie(float a_interval, std::uint32_t a_currentTime) {
        RE::IMenu::AdvanceMovie(a_interval, a_currentTime);
    }

    void SearchMenu::AssignData(std::vector<std::string>& data) {
        auto box = GetSearchBox();
        RE::GFxValue arg;
        view->CreateArray(&arg);
        for (auto& item : data) {
            RE::GFxValue entry;
            view->CreateObject(&entry);
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