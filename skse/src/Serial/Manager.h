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

    void Save(SKSE::SerializationInterface* a_intfc);
    void Load(SKSE::SerializationInterface* a_intfc);
    void Revert(SKSE::SerializationInterface* a_intfc);

    void exportSettings(json& json);
    void importSettings(json& json);
}  // namespace Serialization