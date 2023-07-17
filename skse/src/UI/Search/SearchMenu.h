#pragma once
#include "UI/UIInterop.h"

namespace UI::Search {
    class SearchMenu : public RE::IMenu {
    public:
        using Super = RE::IMenu;

    public:
        static constexpr const char* MENU_PATH = "OStimSearchMenu";
        static constexpr const char* MENU_NAME = "OStimSearchMenu";
        static constexpr std::int8_t SORT_PRIORITY{ 3 };

        SearchMenu();

        static void Register();
        static void Show();
        static void Hide();
        static void AssignData(std::vector<std::string>& data);
        static void Search(std::string value);
        static void SelectOption(std::string val);

        static void ApplyPositions();

        static RE::stl::owner<RE::IMenu*> Creator() { return new SearchMenu(); }

        void AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override;
    private:
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
        inline static RE::GPtr<RE::GFxMovieView> view;
    };

    class doSearchFunction : public RE::GFxFunctionHandler {
    public:
        void Call(Params& args) override {
            UI::Search::SearchMenu::Search(args.args[0].GetString());
        }
    };

    class doSelectOptionFunction : public RE::GFxFunctionHandler {
    public:
        void Call(Params& args) override {
            UI::Search::SearchMenu::SelectOption(args.args[0].GetString());
        }
    };
}