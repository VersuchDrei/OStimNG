#include "SerializationUtil.h"

namespace SerializationUtil {
    void writeString(SKSE::SerializationInterface* serial, std::string value) {
        size_t size = value.size();
        serial->WriteRecordData(&size, sizeof(size));
        for (char c : value) {
            serial->WriteRecordData(&c, sizeof(c));
        }
    }

    std::string readString(SKSE::SerializationInterface* serial) {
        std::vector<char> chars;
        size_t size;
        serial->ReadRecordData(&size, sizeof(size));
        char c;
        for (; size > 0; size--) {
            serial->ReadRecordData(&c, sizeof(c));
            chars.push_back(c);
        }
        return std::string(chars.begin(), chars.end());
    }

    void writeStringMap(SKSE::SerializationInterface* serial, std::unordered_map<std::string, std::string> map) {
        size_t size = map.size();
        serial->WriteRecordData(&size, sizeof(size));
        for (auto& entry : map) {
            writeString(serial, entry.first);
            writeString(serial, entry.second);
        }
    }

    std::unordered_map<std::string, std::string> readStringMap(SKSE::SerializationInterface* serial) {
        std::unordered_map<std::string, std::string> map;

        size_t size;
        serial->ReadRecordData(&size, sizeof(size));
        for (; size > 0; size--) {
            std::string key = readString(serial);
            std::string value = readString(serial);
            map.emplace(key, value);
        }

        return map;
    }
}