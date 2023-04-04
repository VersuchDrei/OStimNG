#pragma once

#include "IncrementValue.h"

#include <Alignment/ActorAlignment.h>
#include <Alignment/Alignments.h>
#include <Core/Thread.h>
#include <Graph/Node.h>
#include <UI/UIInterop.h>

namespace UI::Align {
    class AlignMenu : public RE::IMenu {
    public:
        using Super = RE::IMenu;

    public:
        static constexpr const char* MENU_PATH = "AlignMenu";
        static constexpr const char* MENU_NAME = "AlignMenu";
        static constexpr std::int8_t SORT_PRIORITY{ 3 };

        AlignMenu();

        static void Register();
        static void Show();
        static void Hide();
        static void Update();

        static RE::stl::owner<RE::IMenu*> Creator() { return new AlignMenu(); }

        void AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override;

        static void SetThread(OStim::Thread* thread);

        static void SetNode(Graph::Node* node);        
        static void SetActor(int actor);

        static void Handle(UI::Controls control);

        static void HandleThreadRemoved(OStim::Thread* thread);
    private:
        static void UpdateSceneInfo();
        static void UpdateActorInfo();

        static void SelectField(int field);
        static void ScrollSelectedField(int field);
        static void ToggleActor();
        static void Increment(bool up);
        static void LoadCurrentAlignment();
        static void CycleIncrement();

        class Logger : public RE::GFxLog {
        public:
            void LogMessageVarg(LogMessageType, const char* a_fmt, std::va_list a_argList) override {
                std::string fmt(a_fmt ? a_fmt : "");
                while (!fmt.empty() && fmt.back() == '\n') {
                    fmt.pop_back();
                }

                std::va_list args;
                va_copy(args, a_argList);
                std::vector<char> buf(static_cast<std::size_t>(std::vsnprintf(0, 0, fmt.c_str(), a_argList) + 1));
                std::vsnprintf(buf.data(), buf.size(), fmt.c_str(), args);
                va_end(args);

                logger::info("{}"sv, buf.data());
            }
        };

    private:
        inline static OStim::Thread* currentThread;
        inline static Graph::Node* currentNode;     
        inline static Alignment::ActorAlignment currentActorInfo;

        inline static int selectedField = 0;
        inline static int selectedSlot = 0;
        inline static IncrementValue incrementValue = IncrementValue::ONE;
        
    };
}  // namespace UI::Align