#pragma once

namespace Config {
	class Config {
    public:
        inline static bool autoPPA = true;
        inline static bool verifyTranslationIntegrity = true;

		static void loadConfig();
	};
}