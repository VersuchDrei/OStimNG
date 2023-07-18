#include "AlignMenu.h"

#include "RE/G/GFxValue.h"
#include "RE/G/GPtr.h"

#include <UI/Settings.h>
#include "UI/UIState.h"
#include "Alignment/Alignments.h"

namespace UI::Align {

    inline RE::GFxValue GetRoot() {
        RE::GFxValue root;
        RE::GPtr<RE::IMenu> alignMenu = RE::UI::GetSingleton()->GetMenu(AlignMenu::MENU_NAME);
        assert(alignMenu && alignMenu->uiMovie);
        alignMenu->uiMovie->GetVariable(&root, "_root.rootObj");
        return root;
    }

    AlignMenu::AlignMenu() : Super(MENU_NAME) {}

    void AlignMenu::Show() {
        OStimMenu::Show();
        auto uiState = UI::UIState::GetSingleton();
        if(uiState)
        if (uiState->currentThread != nullptr) {
            SetActor(0);
            SelectField(0);
        }
        UI::Settings::LoadSettings();
        ApplyPositions();
        _isOpen = true;
    }

    void AlignMenu::ThreadChanged(){
        SetActor(0);
    }

    void AlignMenu::NodeChanged(){
        UpdateSceneInfo();
        LoadCurrentAlignment();
        UpdateActorInfo();
    }

    void AlignMenu::SetActor(int actor) {
        selectedSlot = actor;
        LoadCurrentAlignment();
        UpdateActorInfo();
    }

    void AlignMenu::LoadCurrentAlignment() {
        currentActorInfo = UI::UIState::GetSingleton()->currentThread->getActorAlignment(selectedSlot);
    }

    void AlignMenu::Hide() {
        OStimMenu::Hide();
        Alignment::Alignments::SerializeAlignments();
    }

    void AlignMenu::UpdateSceneInfo() {
        auto uiState = UI::UIState::GetSingleton();
        auto currentThread = uiState->currentThread;
        auto currentNode = uiState->currentNode;
        if (currentThread == nullptr || currentNode == nullptr) return;
        auto root = GetRoot();

        RE::GFxValue sceneInfo;
        root.GetMember("sceneInfo", &sceneInfo);

        const RE::GFxValue sceneInfoPack = RE::GFxValue{currentNode->sourceModule.c_str()};
        const RE::GFxValue sceneInfoAnimName = RE::GFxValue{currentNode->scene_id.c_str()};
        //const RE::GFxValue threadKeyValue = RE::GFxValue{currentThread->getAlignmentKey().toString().c_str()};
        std::string key = currentThread->getAlignmentKey();
        const RE::GFxValue threadKeyValue = RE::GFxValue{key.c_str()};

        RE::GFxValue infoArray[3]{sceneInfoPack, sceneInfoAnimName, threadKeyValue};
        sceneInfo.Invoke("updateInfo", nullptr, infoArray, 3);
    }

    void AlignMenu::UpdateActorInfo() {
        if (selectedSlot < 0) {
            return;
        }

        auto root = GetRoot();

        RE::GFxValue alignmentInfo;
        root.GetMember("alignmentInfo", &alignmentInfo);

        const RE::GFxValue actorName = RE::GFxValue{ UI::UIState::GetSingleton()->currentThread->GetActor(selectedSlot)->getActor().getName()};
        const RE::GFxValue actorSlot = selectedSlot;

        auto gender = "*";

        std::string incString = IncrementValueImpl::format(incrementValue);
        const RE::GFxValue incValue = RE::GFxValue{incString};

        const RE::GFxValue actorGender = RE::GFxValue{gender};

        RE::GFxValue infoArray[10]{actorName,
                                  actorSlot,
                                  actorGender,
                                  incValue,
                                  currentActorInfo.offsetX,
                                  currentActorInfo.offsetY,
                                  currentActorInfo.offsetZ,
                                  currentActorInfo.scale,
                                  currentActorInfo.rotation,
                                  currentActorInfo.sosBend};

        alignmentInfo.Invoke("updateInfo", nullptr, infoArray, 10);
    }

    void AlignMenu::Handle(UI::Controls control) {
        switch (control) {
            case Up: {
                ScrollSelectedField(-1);
            } break;
            case Down: {
                ScrollSelectedField(1);
            } break;
            case Toggle: {
                ToggleActor();
            } break;
            case Left: {
                Increment(false);
            } break;
            case Right: {
                Increment(true);
            } break;
            case No: {
                CycleIncrement();
            } break;
        }
    }

    void AlignMenu::ApplyPositions() {
        auto root = GetRoot();
        if (!root.IsObject())
            return;

        auto controlPositions = &UI::Settings::positionSettings.AlignMenuPosition.ControlPosition;
        const RE::GFxValue controlX = RE::GFxValue{ controlPositions->xPos };
        const RE::GFxValue controlY = RE::GFxValue{ controlPositions->yPos };
        const RE::GFxValue controlXScale = RE::GFxValue{ controlPositions->xScale };
        const RE::GFxValue controlYScale = RE::GFxValue{ controlPositions->yScale };
        RE::GFxValue controlPosArray[4]{ controlX, controlY, controlXScale, controlYScale };

        RE::GFxValue alignmentInfo;
        root.GetMember("alignmentInfo", &alignmentInfo);
        alignmentInfo.Invoke("setPosition", nullptr, controlPosArray, 4);


        auto infoPositions = &UI::Settings::positionSettings.AlignMenuPosition.InfoPosition;
        const RE::GFxValue infoX = RE::GFxValue{ infoPositions->xPos };
        const RE::GFxValue infoY = RE::GFxValue{ infoPositions->yPos };
        const RE::GFxValue infoXScale = RE::GFxValue{ infoPositions->xScale };
        const RE::GFxValue infoYScale = RE::GFxValue{ infoPositions->yScale };
        RE::GFxValue infoPosArray[4]{ infoX, infoY, infoXScale, infoYScale };

        RE::GFxValue sceneInfo;
        root.GetMember("sceneInfo", &sceneInfo);
        sceneInfo.Invoke("setPosition", nullptr, infoPosArray, 4);
    }

    void AlignMenu::SelectField(int field) {
        auto root = GetRoot();

        RE::GFxValue alignmentInfo;
        root.GetMember("alignmentInfo", &alignmentInfo);
        RE::GFxValue fieldValue[1]{field};
        alignmentInfo.Invoke("selectField", nullptr, fieldValue, 1);
    }

    void AlignMenu::ScrollSelectedField(int field) {
        if (selectedField + field < 0) {
            selectedField = 5;
        } else if (selectedField + field > 5) {
            selectedField = 0;
        } else {
            selectedField += field;
        }
        SelectField(selectedField);
    }

    void AlignMenu::ToggleActor() {
        selectedSlot++;
        if (selectedSlot > UI::UIState::GetSingleton()->currentNode->actors.size() - 1) {
            selectedSlot = 0;
        }
        SetActor(selectedSlot);

        UpdateActorInfo();
    }

    void AlignMenu::Increment(bool up) {
        auto root = GetRoot();

        float* currentVal;
        switch (selectedField) {
            case 0:
                currentVal = &currentActorInfo.offsetX;
                break;
            case 1:
                currentVal = &currentActorInfo.offsetY;
                break;
            case 2:
                currentVal = &currentActorInfo.offsetZ;
                break;
            case 3:
                currentVal = &currentActorInfo.scale;
                break;
            case 4:
                currentVal = &currentActorInfo.rotation;
                break;
            case 5:
                currentVal = &currentActorInfo.sosBend;
                break;
            default:
                return;
        }
        float actualIncrement = selectedField == 5 ? 1 : IncrementValueImpl::getValue(incrementValue);
        if (up)
            *currentVal += actualIncrement;
        else
            *currentVal -= actualIncrement;

        RE::GFxValue alignmentInfo;
        root.GetMember("alignmentInfo", &alignmentInfo);
        RE::GFxValue values[2]{*currentVal, up};
        alignmentInfo.Invoke("updateDoubleField", nullptr, values, 2);

        UI::UIState::GetSingleton()->currentThread->updateActorAlignment(selectedSlot, currentActorInfo);
    }

    void AlignMenu::CycleIncrement() {
        incrementValue = IncrementValueImpl::loop(incrementValue);
        auto root = GetRoot();
        RE::GFxValue alignmentInfo;
        root.GetMember("alignmentInfo", &alignmentInfo);
        std::string incString = IncrementValueImpl::format(incrementValue);
        RE::GFxValue values[1]{RE::GFxValue{incString}};
        alignmentInfo.Invoke("updateIncrement", nullptr, values, 1);
    }
}  // namespace UI::Align