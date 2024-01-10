#pragma once

#include "ActorData.h"
#include "OldThread.h"

namespace Serialization {
    inline const auto undressingMaskRecord = _byteswap_ulong('UDRM');
    inline const auto oldThreadsRecord = _byteswap_ulong('OLTH');
    inline const auto actorDataRecord = _byteswap_ulong('ACDT');

    inline uint32_t deserializationErrors = 0;

    inline std::mutex _lock;

    inline std::vector<OldThread> oldThreads;
    inline std::unordered_map<RE::FormID, ActorData> actorData;

    void closeOldThreads();
    
    std::string getEquipObject(RE::FormID formID, std::string type);
    void setEquipObject(RE::FormID formID, std::string type, std::string id);

    RE::FormID getVoiceSet(RE::FormID formID);
    void setVoiceSet(RE::FormID formID, RE::FormID voice);

    float getActionActorStimulation(RE::FormID formID, std::string action);
    void setActionActorStimulation(RE::FormID formID, std::string action, float stimulation);
    float getActionActorMaxStimulation(RE::FormID formID, std::string action);
    void setActionActorMaxStimulation(RE::FormID formID, std::string action, float stimulation);
    float getActionTargetStimulation(RE::FormID formID, std::string action);
    void setActionTargetStimulation(RE::FormID formID, std::string action, float stimulation);
    float getActionTargetMaxStimulation(RE::FormID formID, std::string action);
    void setActionTargetMaxStimulation(RE::FormID formID, std::string action, float stimulation);
    float getActionPerformerStimulation(RE::FormID formID, std::string action);
    void setActionPerformerStimulation(RE::FormID formID, std::string action, float stimulation);
    float getActionPerformerMaxStimulation(RE::FormID formID, std::string action);
    void setActionPerformerMaxStimulation(RE::FormID formID, std::string action, float stimulation);

    float getEventActorStimulation(RE::FormID formID, std::string ev);
    void setEventActorStimulation(RE::FormID formID, std::string ev, float stimulation);
    float getEventActorMaxStimulation(RE::FormID formID, std::string ev);
    void setEventActorMaxStimulation(RE::FormID formID, std::string ev, float stimulation);
    float getEventTargetStimulation(RE::FormID formID, std::string ev);
    void setEventTargetStimulation(RE::FormID formID, std::string ev, float stimulation);
    float getEventTargetMaxStimulation(RE::FormID formID, std::string ev);
    void setEventTargetMaxStimulation(RE::FormID formID, std::string ev, float stimulation);
    float getEventPerformerStimulation(RE::FormID formID, std::string ev);
    void setEventPerformerStimulation(RE::FormID formID, std::string ev, float stimulation);
    float getEventPerformerMaxStimulation(RE::FormID formID, std::string ev);
    void setEventPerformerMaxStimulation(RE::FormID formID, std::string ev, float stimulation);

    void Save(SKSE::SerializationInterface* a_intfc);
    void Load(SKSE::SerializationInterface* a_intfc);
    void Revert(SKSE::SerializationInterface* a_intfc);

    void exportSettings(json& json);
    void importSettings(json& json);
}  // namespace Serialization