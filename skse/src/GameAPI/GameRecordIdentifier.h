#pragma once

namespace GameAPI {
    struct GameRecordIdentifier {
        std::string mod = "";
        RE::FormID formID = 0;

        std::string toString();
        void readString(std::string& string);
    };
}