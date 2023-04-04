#include "Serial/Manager.h"

#include "OldThread.h"

#include "Core/ThreadManager.h"
#include "Game/Locker.h"
#include "Util/MCMTable.h"

namespace Serialization {
    void closeOldThreads() {
        for (OldThread thread : oldThreads) {
            thread.close();
        }
        oldThreads.clear();
    }

    std::string getEquipObject(RE::FormID formID, std::string type) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.equipObjects.find(type);
            if (iter2 != iter->second.equipObjects.end()) {
                return iter2->second;
            }
        }

        return "";
    }

    void setEquipObject(RE::FormID formID, std::string type, std::string id) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.equipObjects[type] = id;
        } else {
            actorData[formID] = {.equipObjects = {{type, id}}};
        }
    }

    void Save(SKSE::SerializationInterface* serial) {
        std::unique_lock lock(_lock);
        logger::info("serializing data");

        if (!serial->OpenRecord(undressingMaskRecord, 0)) {
            logger::error("Unable to open record to write cosave data.");
            return;
        }

        uint32_t undressingMask = MCM::MCMTable::getUndressingMask();
        serial->WriteRecordData(&undressingMask, sizeof(undressingMask));

        if (!serial->OpenRecord(oldThreadsRecord, 0)) {
            logger::error("Unable to open record to write cosave data.");
            return;
        }

        std::vector<OldThread> oldThreads = OStim::ThreadManager::GetSingleton()->serialize();
        size_t size = oldThreads.size();
        serial->WriteRecordData(&size, sizeof(size));
        for (OldThread oldThread : oldThreads) {
            oldThread.serialize(serial);
        }

        if (!serial->OpenRecord(actorDataRecord, 0)) {
            logger::error("Unable to open record to write cosave data.");
            return;
        }

        size = actorData.size();
        serial->WriteRecordData(&size, sizeof(size));
        for (auto& [id, data] : actorData) {
            serial->WriteRecordData(&id, sizeof(id));
            data.serialize(serial);
        }
    }

    void Load(SKSE::SerializationInterface* serial) {
        std::unique_lock lock(_lock);
        logger::info("deserializing data");

        actorData.clear();

        std::uint32_t type;
        std::uint32_t size;
        std::uint32_t version;

        while (serial->GetNextRecordInfo(type, version, size)) {
            if (type == undressingMaskRecord) {
                uint32_t undressingMask;
                serial->ReadRecordData(&undressingMask, sizeof(undressingMask));
                MCM::MCMTable::setUndressingMask(undressingMask);
            } else if (type == oldThreadsRecord) {
                size_t size;
                serial->ReadRecordData(&size, sizeof(size));
                deserializationErrors = 0;
                for (int i = 0; i < size; i++) {
                    OldThread::deserialize(serial, oldThreads, deserializationErrors);
                }
            } else if (type == actorDataRecord){
                size_t size;
                serial->ReadRecordData(&size, sizeof(size));
                for (int i = 0; i < size; i++) {
                    RE::FormID oldID;
                    serial->ReadRecordData(&oldID, sizeof(oldID));
                    RE::FormID newID;
                    bool valid = serial->ResolveFormID(oldID, newID);

                    ActorData data = ActorData::deserialize(serial);

                    if (valid) {
                        actorData.emplace(newID, data);
                    } else {
                        logger::warn("cannot resolve actor id {:x}, missing mod?", oldID);
                    }
                }
            } else {
                logger::warn("Unknown record type in cosave.");
            }
        }
    }

    void Revert(SKSE::SerializationInterface* serial) {
        std::unique_lock lock(_lock);
        auto locker = Locker::GetSingleton();
        locker->Revert(serial);
        OStim::ThreadManager::GetSingleton()->UntrackAllThreads();
        MCM::MCMTable::resetDefaults();
    }

    void exportSettings(json& json) {
        json["actorData"] = json::object();

        for (auto& [formID, data] : actorData) {
            std::string stringID = std::to_string(formID);
            json["actorData"][stringID] = json::object();

            RE::TESForm* form = RE::TESForm::LookupByID(formID);
            if (form) {
                json["actorData"][stringID]["file"] = form->GetFile(0)->GetFilename();
            }

            if (!data.equipObjects.empty()) {
                json["actorData"][stringID]["equipObjects"] = json::object();
                for (auto& [type, id] : data.equipObjects) {
                    json["actorData"][stringID]["equipObjects"][type] = id;
                }
            }
        }
    }

    void importSettings(json& json) {
        actorData.clear();

        RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
        if (json.contains("actorData")) {
            for (auto& [key, value] : json["actorData"].items()) {
                RE::FormID formID = std::stoi(key);

                if (value.contains("file")) {
                    std::string file = value["file"];
                    const RE::TESFile* mod = dataHandler->LookupLoadedModByName(file);
                    if (mod) {
                        formID &= 0x00FFFFFF;
                        formID += mod->GetCompileIndex() << 24;
                    } else {
                        mod = dataHandler->LookupLoadedLightModByName(file);
                        if (mod) {
                            formID &= 0x00000FFF;
                            formID += mod->GetPartialIndex() << 12;
                        }
                    }
                }

                ActorData data;

                if (value.contains("equipObjects")) {
                    for (auto& [type, id] : value["equipObjects"].items()) {
                        data.equipObjects[type] = id;
                    }
                }

                actorData[formID] = data;
            }
        }
    }
}  // namespace Serialization