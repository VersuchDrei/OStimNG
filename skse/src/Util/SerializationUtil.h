#pragma once

namespace SerializationUtil {
    void writeString(SKSE::SerializationInterface* serial, std::string value);
    std::string readString(SKSE::SerializationInterface* serial);

    void writeStringMap(SKSE::SerializationInterface* serial, std::unordered_map<std::string, std::string> map);
    std::unordered_map<std::string, std::string> readStringMap(SKSE::SerializationInterface* serial);
}