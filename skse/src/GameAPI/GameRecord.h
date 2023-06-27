#pragma once

#include "GamePointer.h"

namespace GameAPI {
    template<class T>
    struct GameRecord {
    public:
        T* form = nullptr;

        inline operator bool() const { return form; }
        inline bool operator==(const GameRecord<T> other) { return form == other.form; }
        inline bool operator!=(const GameRecord<T> other) { return form != other.form; }

        inline void loadJson(std::string path, json json) {
            if (!json.contains("mod")) {
                logger::info("file {} does not have field 'mod' defined", path);
                return;
            }

            if (!json.contains("formid")) {
                logger::info("file {} does not have field 'formid' defined", path);
                return;
            }

            std::string mod = json["mod"];
            std::string formid = json["formid"];
            loadFile(mod, std::stoi(formid, nullptr, 16));
            if (!form) {
                logger::info(" file {} contains unkown form {}|{}", path, mod, formid);
            }
        }

        inline void loadFile(std::string mod, uint32_t formID) {form = RE::TESDataHandler::GetSingleton()->LookupForm<T>(formID, mod); }

        inline uint32_t getFormID() const {return form->formID; }
    };
}