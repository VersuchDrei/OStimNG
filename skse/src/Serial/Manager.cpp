#include "Serial/Manager.h"

#include "OldThread.h"

#include "Core/ThreadManager.h"
#include "Game/Locker.h"
#include "MCM/MCMTable.h"
#include "Util/Globals.h"
#include "Util/MathUtil.h"

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


    RE::FormID getVoiceSet(RE::FormID formID) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            return iter->second.voiceSet;
        }

        return 0;
    }

    void setVoiceSet(RE::FormID formID, RE::FormID voiceSet) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.voiceSet = voiceSet;
        } else {
            actorData[formID] = {.voiceSet = voiceSet};
        }
    }


    float getActionActorStimulation(RE::FormID formID, std::string action) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.actionActorStimulations.find(action);
            if (iter2 != iter->second.actionActorStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setActionActorStimulation(RE::FormID formID, std::string action, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.actionActorStimulations[action] = stimulation;
        } else {
            actorData[formID] = {.actionActorStimulations = {{action, stimulation}}};
        }
    }

    float getActionActorMaxStimulation(RE::FormID formID, std::string action) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.actionActorMaxStimulations.find(action);
            if (iter2 != iter->second.actionActorMaxStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setActionActorMaxStimulation(RE::FormID formID, std::string action, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.actionActorMaxStimulations[action] = stimulation;
        } else {
            actorData[formID] = {.actionActorMaxStimulations = {{action, stimulation}}};
        }
    }

    float getActionTargetStimulation(RE::FormID formID, std::string action) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.actionTargetStimulations.find(action);
            if (iter2 != iter->second.actionTargetStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setActionTargetStimulation(RE::FormID formID, std::string action, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.actionTargetStimulations[action] = stimulation;
        } else {
            actorData[formID] = {.actionTargetStimulations = {{action, stimulation}}};
        }
    }

    float getActionTargetMaxStimulation(RE::FormID formID, std::string action) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.actionTargetMaxStimulations.find(action);
            if (iter2 != iter->second.actionTargetMaxStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setActionTargetMaxStimulation(RE::FormID formID, std::string action, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.actionTargetMaxStimulations[action] = stimulation;
        } else {
            actorData[formID] = {.actionTargetMaxStimulations = {{action, stimulation}}};
        }
    }

    float getActionPerformerStimulation(RE::FormID formID, std::string action) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.actionPerformerStimulations.find(action);
            if (iter2 != iter->second.actionPerformerStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setActionPerformerStimulation(RE::FormID formID, std::string action, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.actionPerformerStimulations[action] = stimulation;
        } else {
            actorData[formID] = {.actionPerformerStimulations = {{action, stimulation}}};
        }
    }

    float getActionPerformerMaxStimulation(RE::FormID formID, std::string action) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.actionPerformerMaxStimulations.find(action);
            if (iter2 != iter->second.actionPerformerMaxStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setActionPerformerMaxStimulation(RE::FormID formID, std::string action, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.actionPerformerMaxStimulations[action] = stimulation;
        } else {
            actorData[formID] = {.actionPerformerMaxStimulations = {{action, stimulation}}};
        }
    }


    float getEventActorStimulation(RE::FormID formID, std::string ev) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.eventActorStimulations.find(ev);
            if (iter2 != iter->second.eventActorStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setEventActorStimulation(RE::FormID formID, std::string ev, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.eventActorStimulations[ev] = stimulation;
        } else {
            actorData[formID] = {.eventActorStimulations = {{ev, stimulation}}};
        }
    }

    float getEventActorMaxStimulation(RE::FormID formID, std::string ev) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.eventActorMaxStimulations.find(ev);
            if (iter2 != iter->second.eventActorMaxStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setEventActorMaxStimulation(RE::FormID formID, std::string ev, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.eventActorMaxStimulations[ev] = stimulation;
        } else {
            actorData[formID] = {.eventActorMaxStimulations = {{ev, stimulation}}};
        }
    }

    float getEventTargetStimulation(RE::FormID formID, std::string ev) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.eventTargetStimulations.find(ev);
            if (iter2 != iter->second.eventTargetStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setEventTargetStimulation(RE::FormID formID, std::string ev, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.eventTargetStimulations[ev] = stimulation;
        } else {
            actorData[formID] = {.eventTargetStimulations = {{ev, stimulation}}};
        }
    }

    float getEventTargetMaxStimulation(RE::FormID formID, std::string ev) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.eventTargetMaxStimulations.find(ev);
            if (iter2 != iter->second.eventTargetMaxStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setEventTargetMaxStimulation(RE::FormID formID, std::string ev, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.eventTargetMaxStimulations[ev] = stimulation;
        } else {
            actorData[formID] = {.eventTargetMaxStimulations = {{ev, stimulation}}};
        }
    }

    float getEventPerformerStimulation(RE::FormID formID, std::string ev) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.eventPerformerStimulations.find(ev);
            if (iter2 != iter->second.eventPerformerStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setEventPerformerStimulation(RE::FormID formID, std::string ev, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.eventPerformerStimulations[ev] = stimulation;
        } else {
            actorData[formID] = {.eventPerformerStimulations = {{ev, stimulation}}};
        }
    }

    float getEventPerformerMaxStimulation(RE::FormID formID, std::string ev) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            auto iter2 = iter->second.eventPerformerMaxStimulations.find(ev);
            if (iter2 != iter->second.eventPerformerMaxStimulations.end()) {
                return iter2->second;
            }
        }
        return std::numeric_limits<float>::quiet_NaN();
    }

    void setEventPerformerMaxStimulation(RE::FormID formID, std::string ev, float stimulation) {
        auto iter = actorData.find(formID);
        if (iter != actorData.end()) {
            iter->second.eventPerformerMaxStimulations[ev] = stimulation;
        } else {
            actorData[formID] = {.eventPerformerMaxStimulations = {{ev, stimulation}}};
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

        if (!serial->OpenRecord(actorDataRecord, ACTOR_DATA_VERSION)) {
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

                    ActorData data = ActorData::deserialize(serial, version);

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
        Util::Globals::resetSaveGameValues();
        MCM::MCMTable::resetValues();
    }

    void exportSettings(json& json) {
        json["actorData"] = json::object();

        for (auto& [formID, data] : actorData) {
            std::string stringID = std::to_string(MathUtil::uintToInt(formID));
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

            if (data.voiceSet != 0) {
                RE::TESForm* voice = RE::TESForm::LookupByID(formID);
                if (voice) {
                    json["actorData"][stringID]["voice"] = json::object();
                    json["actorData"][stringID]["voice"]["mod"] = voice->GetFile(0)->GetFilename();
                    json["actorData"][stringID]["voice"]["formid"] = std::to_string(data.voiceSet & 0x00FFFFFF);
                }
            }

            if (!data.actionActorStimulations.empty()) {
                json["actorData"][stringID]["actionActorStimulations"] = json::object();
                for (auto& [action, stim] : data.actionActorStimulations) {
                    json["actorData"][stringID]["actionActorStimulations"][action] = stim;
                }
            }
            if (!data.actionActorMaxStimulations.empty()) {
                json["actorData"][stringID]["actionActorMaxStimulations"] = json::object();
                for (auto& [action, stim] : data.actionActorMaxStimulations) {
                    json["actorData"][stringID]["actionActorMaxStimulations"][action] = stim;
                }
            }
            if (!data.actionTargetStimulations.empty()) {
                json["actorData"][stringID]["actionTargetStimulations"] = json::object();
                for (auto& [action, stim] : data.actionTargetStimulations) {
                    json["actorData"][stringID]["actionTargetStimulations"][action] = stim;
                }
            }
            if (!data.actionTargetMaxStimulations.empty()) {
                json["actorData"][stringID]["actionTargetMaxStimulations"] = json::object();
                for (auto& [action, stim] : data.actionTargetMaxStimulations) {
                    json["actorData"][stringID]["actionTargetMaxStimulations"][action] = stim;
                }
            }
            if (!data.actionPerformerStimulations.empty()) {
                json["actorData"][stringID]["actionPerformerStimulations"] = json::object();
                for (auto& [action, stim] : data.actionPerformerStimulations) {
                    json["actorData"][stringID]["actionPerformerStimulations"][action] = stim;
                }
            }
            if (!data.actionPerformerMaxStimulations.empty()) {
                json["actorData"][stringID]["actionPerformerMaxStimulations"] = json::object();
                for (auto& [action, stim] : data.actionPerformerMaxStimulations) {
                    json["actorData"][stringID]["actionPerformerMaxStimulations"][action] = stim;
                }
            }

            if (!data.eventActorStimulations.empty()) {
                json["actorData"][stringID]["eventActorStimulations"] = json::object();
                for (auto& [ev, stim] : data.eventActorStimulations) {
                    json["actorData"][stringID]["eventActorStimulations"][ev] = stim;
                }
            }
            if (!data.eventActorMaxStimulations.empty()) {
                json["actorData"][stringID]["eventActorMaxStimulations"] = json::object();
                for (auto& [ev, stim] : data.eventActorMaxStimulations) {
                    json["actorData"][stringID]["eventActorMaxStimulations"][ev] = stim;
                }
            }
            if (!data.eventTargetStimulations.empty()) {
                json["actorData"][stringID]["eventTargetStimulations"] = json::object();
                for (auto& [ev, stim] : data.eventTargetStimulations) {
                    json["actorData"][stringID]["eventTargetStimulations"][ev] = stim;
                }
            }
            if (!data.eventTargetMaxStimulations.empty()) {
                json["actorData"][stringID]["eventTargetMaxStimulations"] = json::object();
                for (auto& [ev, stim] : data.eventTargetMaxStimulations) {
                    json["actorData"][stringID]["eventTargetMaxStimulations"][ev] = stim;
                }
            }
            if (!data.eventPerformerStimulations.empty()) {
                json["actorData"][stringID]["eventPerformerStimulations"] = json::object();
                for (auto& [ev, stim] : data.eventPerformerStimulations) {
                    json["actorData"][stringID]["eventPerformerStimulations"][ev] = stim;
                }
            }
            if (!data.eventPerformerMaxStimulations.empty()) {
                json["actorData"][stringID]["eventPerformerMaxStimulations"] = json::object();
                for (auto& [ev, stim] : data.eventPerformerMaxStimulations) {
                    json["actorData"][stringID]["eventPerformerMaxStimulations"][ev] = stim;
                }
            }
        }
    }

    void importSettings(json& json) {
        actorData.clear();

        RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
        if (json.contains("actorData")) {
            for (auto& [key, value] : json["actorData"].items()) {
                RE::FormID formID = MathUtil::intToUint(std::stoi(key));

                if (value.contains("file")) {
                    std::string file = value["file"];
                    if (const RE::TESFile* mod = dataHandler->LookupLoadedModByName(file)) {
                        formID &= 0x00FFFFFF;
                        formID += mod->GetCompileIndex() << 24;
                    } else if (const RE::TESFile* mod = dataHandler->LookupLoadedLightModByName(file)) {
                        formID &= 0x00000FFF;
                        formID += mod->GetPartialIndex() << 12;
                    }
                }

                ActorData data;

                if (value.contains("equipObjects")) {
                    for (auto& [type, id] : value["equipObjects"].items()) {
                        data.equipObjects[type] = id;
                    }
                }

                if (value.contains("voice")) {
                    RE::FormID voiceID = std::stoi(static_cast<std::string>(value["voice"]["formid"]));
                    if (const RE::TESFile* mod = dataHandler->LookupLoadedModByName(value["voice"]["mod"])) {
                        voiceID &= 0x00FFFFFF;
                        voiceID += mod->GetCompileIndex() << 24;
                    } else if (const RE::TESFile* mod = dataHandler->LookupLoadedLightModByName(value["voice"]["mod"])) {
                        voiceID &= 0x00000FFF;
                        voiceID += mod->GetPartialIndex() << 12;
                    }
                    data.voiceSet = voiceID;
                }

                if (value.contains("actionActorStimulations")) {
                    for (auto& [action, stim] : value["actionActorStimulations"].items()) {
                        data.actionActorStimulations[action] = stim;
                    }
                }
                if (value.contains("actionActorMaxStimulations")) {
                    for (auto& [action, stim] : value["actionActorMaxStimulations"].items()) {
                        data.actionActorMaxStimulations[action] = stim;
                    }
                }
                if (value.contains("actionTargetStimulations")) {
                    for (auto& [action, stim] : value["actionTargetStimulations"].items()) {
                        data.actionTargetStimulations[action] = stim;
                    }
                }
                if (value.contains("actionTargetMaxStimulations")) {
                    for (auto& [action, stim] : value["actionTargetMaxStimulations"].items()) {
                        data.actionTargetMaxStimulations[action] = stim;
                    }
                }
                if (value.contains("actionPerformerStimulations")) {
                    for (auto& [action, stim] : value["actionPerformerStimulations"].items()) {
                        data.actionPerformerStimulations[action] = stim;
                    }
                }
                if (value.contains("actionPerformerMaxStimulations")) {
                    for (auto& [action, stim] : value["actionPerformerMaxStimulations"].items()) {
                        data.actionPerformerMaxStimulations[action] = stim;
                    }
                }

                if (value.contains("eventActorStimulations")) {
                    for (auto& [ev, stim] : value["eventActorStimulations"].items()) {
                        data.eventActorStimulations[ev] = stim;
                    }
                }
                if (value.contains("eventActorMaxStimulations")) {
                    for (auto& [ev, stim] : value["eventActorMaxStimulations"].items()) {
                        data.eventActorMaxStimulations[ev] = stim;
                    }
                }
                if (value.contains("eventTargetStimulations")) {
                    for (auto& [ev, stim] : value["eventTargetStimulations"].items()) {
                        data.eventTargetStimulations[ev] = stim;
                    }
                }
                if (value.contains("eventTargetMaxStimulations")) {
                    for (auto& [ev, stim] : value["eventTargetMaxStimulations"].items()) {
                        data.eventTargetMaxStimulations[ev] = stim;
                    }
                }
                if (value.contains("eventPerformerStimulations")) {
                    for (auto& [ev, stim] : value["eventPerformerStimulations"].items()) {
                        data.eventPerformerStimulations[ev] = stim;
                    }
                }
                if (value.contains("eventPerformerMaxStimulations")) {
                    for (auto& [ev, stim] : value["eventPerformerMaxStimulations"].items()) {
                        data.eventPerformerMaxStimulations[ev] = stim;
                    }
                }

                actorData[formID] = data;
            }
        }
    }
}  // namespace Serialization