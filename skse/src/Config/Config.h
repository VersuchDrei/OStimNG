#pragma once

namespace Config {
	class Config {
    public:
        inline static bool automaticGenitalAlignment = true;
        inline static bool verifyTranslationIntegrity = true;

		static void loadConfig();
	};
}