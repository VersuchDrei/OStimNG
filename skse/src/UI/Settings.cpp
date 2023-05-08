#include "UI/Settings.h"
#include "UI/Align/AlignMenu.h"
#include "UI/Scene/SceneMenu.h"
#include <Util.h>

namespace UI {
	const char* SETTINGS_FILE_PATH{ "Data/SKSE/Plugins/OStim/uisettings.json" };

	void LoadPosition(json& json, Position& setting) {
		if (json.contains("x"))
			setting.xPos = json["x"];
		if (json.contains("y"))
			setting.yPos = json["y"];
		if (json.contains("xScale"))
			setting.xScale = json["xScale"];
		if (json.contains("yScale"))
			setting.yScale = json["yScale"];
	}
	void CreateDefaultLocationSettings(json& json) {
		json["x"] = 0;
		json["y"] = 0;
		json["xScale"] = 100;
		json["yScale"] = 100;
	}
	void CreateDefaultSettingsFile(json& json) {

		json["scene"] = json::object();		
		json["scene"]["control"] = json::object();
		CreateDefaultLocationSettings(json["scene"]["control"]);
		json["scene"]["bars"] = json::object();
		CreateDefaultLocationSettings(json["scene"]["bars"]);

		json["align"] = json::object();
		json["align"]["control"] = json::object();
		CreateDefaultLocationSettings(json["align"]["control"]);
		json["align"]["info"] = json::object();
		CreateDefaultLocationSettings(json["align"]["info"]);
	}
	void Settings::LoadSettings() {
		logger::info("loading ui settings");
		auto settingsPath = util::ui_settings_path();
		if (!fs::exists(*settingsPath)) {
			logger::info("ui settings file does not exist, creating");
			json json = json::object();
			CreateDefaultSettingsFile(json);
			auto ostimPath = util::ostim_path();
			std::filesystem::create_directory(*ostimPath);
			std::ofstream settingsFile(*settingsPath);
			settingsFile << std::setw(2) << json << std::endl;
		}
			
		std::ifstream ifs(*settingsPath);
		json json = json::parse(ifs, nullptr, false);

		if (json.is_discarded()) {
			logger::warn("ui settings file is malformed");
			return;
		}
		if (!json.contains("align")) {
			logger::warn("settings file malformed - missing align");
		} else {
			auto& align = json["align"];
			if (!align.contains("control")) {
				logger::warn("settings file malformed - missing align.control");
			} else {
				auto& control = align["control"];
				LoadPosition(control, positionSettings.AlignMenuPosition.ControlPosition);
			}
			if (!align.contains("info")) {
				logger::warn("settings file malformed - missing align.info");
			}
			else {
				auto& info = align["info"];
				LoadPosition(info, positionSettings.AlignMenuPosition.InfoPosition);
			}
		}

		if (!json.contains("scene")) {
			logger::warn("settings file maformed - missing scene");
		}
		else {
			auto& scene = json["scene"];
			if (!scene.contains("control")) {
				logger::warn("settings file malformed - missing scene.control");
			}
			else {
				auto& control = scene["control"];
				LoadPosition(control, positionSettings.ScenePositions.ControlPosition);
			}
			if (!scene.contains("bars")) {
				logger::warn("settings file malformed - missing scene.bars");
			}
			else {
				auto& info = scene["bars"];
				LoadPosition(info, positionSettings.ScenePositions.BarsPosition);
			}
		}
	}
}