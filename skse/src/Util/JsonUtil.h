#pragma once

namespace JsonUtil {
    void loadString(json& json, std::string& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists);
    void loadFloat(json& json, float& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists);
    void loadInt(json& json, int& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists);
    void loadBool(json& json, bool& value, std::string propertyName, std::string& objectName, std::string objectType, bool warnIfNotExists);

	template <class T>
	void loadGameRecord(json& json, T& value, std::string propertyName, std::string& objectName, std::string objectType, std::string& filepath, bool warnIfNotExists) {
        if (json.contains(propertyName)) {
            if (json[propertyName].is_object()) {
                value.loadJson(filepath, json[propertyName]);
            } else {
                logger::warn("property '{}' of {} {} is malformed", propertyName, objectType, objectName);
            }
        } else if (warnIfNotExists) {
            logger::warn("{} doesn't have property '{}' defined", objectName, propertyName);
        }
	}

	template <class T>
	T* getForm(std::string path, json json, RE::TESDataHandler* handler) {
        if (!json.contains("mod")) {
            logger::info("file {} does not have field 'mod' defined", path);
            return nullptr;
		}

		if (!json.contains("formid")) {
            logger::info("file {} does not have field 'formid' defined", path);
            return nullptr;
		}

		std::string mod = json["mod"];
        std::string formid = json["formid"];
		T* form = handler->LookupForm<T>(std::stoi(formid, nullptr, 16), mod);
        if (!form) {
            logger::info(" file {} contains unkown form {}|{}", path, mod, formid);
		}

		return form;
	}

	template <class T>
	T* getForm(std::string path, json json) {
        return getForm<T>(path, json, RE::TESDataHandler::GetSingleton());
	}
}