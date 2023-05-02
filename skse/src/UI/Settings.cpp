#include "UI/Settings.h"

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

	void Settings::LoadSettings() {
		logger::info("loading ui settings");
		fs::path rootPath{ SETTINGS_FILE_PATH };
		if (!fs::exists(rootPath))
			logger::info("filepath ({}) does not exist", SETTINGS_FILE_PATH);
		std::ifstream ifs(SETTINGS_FILE_PATH);
		json json = json::parse(ifs, nullptr, false);

		if (json.is_discarded()) {
			logger::warn("file {} is malformed", SETTINGS_FILE_PATH);
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

	void Settings::ApplyPositions() {

	}
}