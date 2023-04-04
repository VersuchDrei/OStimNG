#include "AlignMenu.h"

#include "RE/G/GFxValue.h"
#include "RE/G/GPtr.h"

#include "Alignment/Alignments.h"

namespace UI::Align {

    inline RE::GFxValue GetRoot() {
        RE::GFxValue root;
        RE::GPtr<RE::IMenu> alignMenu = RE::UI::GetSingleton()->GetMenu(AlignMenu::MENU_NAME);
        if (!alignMenu || !alignMenu->uiMovie) {
            return root;
        }
        alignMenu->uiMovie->GetVariable(&root, "_root.rootObj");
        return root;
    }

    AlignMenu::AlignMenu() : Super() {
        auto scaleformManager = RE::BSScaleformManager::GetSingleton();

        inputContext = Context::kNone;
        depthPriority = 0;

        auto menu = static_cast<Super*>(this);
        menu->depthPriority = SORT_PRIORITY;

        menuFlags.set(RE::UI_MENU_FLAGS::kAlwaysOpen);
        menuFlags.set(RE::UI_MENU_FLAGS::kRequiresUpdate);

        if (uiMovie) {
            uiMovie->SetMouseCursorCount(0);  // disable input
        }

        scaleformManager->LoadMovieEx(this, MENU_PATH, [](RE::GFxMovieDef* a_def) -> void {
            a_def->SetState(RE::GFxState::StateType::kLog, RE::make_gptr<Logger>().get());
        });
    }

    void AlignMenu::Register() {
        auto ui = RE::UI::GetSingleton();
        if (ui) {
            ui->Register(MENU_NAME, Creator);

            AlignMenu::Show();
        }
    }

    void AlignMenu::Show() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(AlignMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
        }
        if (currentThread != nullptr) {
            SetActor(0);
            SelectField(0);
        }
    }

    void AlignMenu::SetThread(OStim::Thread* thread) {
        currentThread = thread;
        SetActor(0);
    }

    void AlignMenu::SetNode(Graph::Node* node) {
        currentNode = node;
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
        currentActorInfo = currentThread->getActorAlignment(selectedSlot);
    }

    void AlignMenu::HandleThreadRemoved(OStim::Thread* thread) {
        if (thread == currentThread) {
            currentThread = nullptr;
        }
    }

    void AlignMenu::Hide() {
        auto msgQ = RE::UIMessageQueue::GetSingleton();
        if (msgQ) {
            msgQ->AddMessage(AlignMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        }

        Alignment::Alignments::SerializeAlignments();
    }

    void AlignMenu::UpdateSceneInfo() {
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

        const RE::GFxValue actorName = RE::GFxValue{currentThread->GetActor(selectedSlot)->getActor()->GetDisplayFullName()};
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

    void AlignMenu::Update() {}

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
        if (selectedSlot > currentNode->actors.size() - 1) {
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

        currentThread->updateActorAlignment(selectedSlot, currentActorInfo);
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

    void AlignMenu::AdvanceMovie(float a_interval, std::uint32_t a_currentTime) {
        AlignMenu::Update();
        RE::IMenu::AdvanceMovie(a_interval, a_currentTime);
    }
}  // namespace UI::Align