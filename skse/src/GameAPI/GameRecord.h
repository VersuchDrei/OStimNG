#pragma once

#include "GameSerializationInterface.h"

namespace GameAPI {
    template<class T>
    struct GameRecord {
    public:
        T* form = nullptr;

        inline operator bool() const { return form; }
        inline bool operator==(const GameRecord<T> other) { return form == other.form; }
        inline bool operator!=(const GameRecord<T> other) { return form != other.form; }

        void loadJson(std::string& path, json json) {
            if (!json.is_object()) {
                logger::warn("form definition in file {} is malformed", path);
            }

            if (!json.contains("mod")) {
                logger::warn("file {} does not have field 'mod' defined", path);
                return;
            }

            if (!json["mod"].is_string()) {
                logger::warn("field 'mod' of file {} isn't a string", path);
                return;
            }

            if (!json.contains("formid")) {
                logger::warn("file {} does not have field 'formid' defined", path);
                return;
            }

            if (!json["formid"].is_string()) {
                logger::warn("field 'formid' of file {} isn't a string, hexadecimal numbers have to be given in string format", path);
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

        void loadSerial(GameSerializationInterface serial) {
            RE::FormID oldFormID;
            RE::FormID newFormID;

            serial.object->ReadRecordData(&oldFormID, sizeof(oldFormID));
            if (oldFormID == 0) {
                form = nullptr;
                return;
            }

            if (!serial.object->ResolveFormID(oldFormID, newFormID)) {
                logger::warn("cannot resolve form id {:x}, missing mod?", oldFormID);
                return;
            }

            form = RE::TESForm::LookupByID<T>(newFormID);
            if (!form) {
                logger::warn("cannot find form with form id {:x}", newFormID);
            }
        }

        void writeSerial(GameSerializationInterface serial) {
            RE::FormID formID = form ? form->GetFormID() : 0;
            serial.object->WriteRecordData(&formID, sizeof(formID));
        }

        inline uint32_t getFormID() const {return form->formID; }
    };
}