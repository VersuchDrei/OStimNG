#pragma once

namespace JsonUtil {
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