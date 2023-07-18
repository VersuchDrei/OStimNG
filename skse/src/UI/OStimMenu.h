#pragma once

namespace UI {
	class OStimMenu : public RE::IMenu {
	
	public:
		OStimMenu(std::string menuName);
		static void Register(std::string menuName, RE::UI::Create_t* createFn);
		void Show();
		void Hide();
		virtual void Update() {};
		virtual void AdvanceMovie(float a_interval, std::uint32_t a_currentTime) override;

	public:
		std::string menuName;

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
	protected:

		bool _isOpen = false;
		RE::GPtr<RE::GFxMovieView> _view;

		using Lock = std::recursive_mutex;
		using Locker = std::lock_guard<Lock>;
		mutable Lock _lock;

	};

}
