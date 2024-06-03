#include "GameRecordIdentifier.h"

#include "Util/StringUtil.h"

namespace GameAPI {
    std::string GameRecordIdentifier::toString() {
        RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();
        if (handler->GetLoadedModIndex(mod)) {
            return mod + "|" + std::to_string(formID & 0xFFFFFF);
        } else if (handler->GetLoadedLightModIndex(mod)) {
            return mod + "|" + std::to_string(formID & 0xFFF);
        }

        return "";
    }

    void GameRecordIdentifier::readString(std::string& string) {
        std::vector<std::string> stringVec = StringUtil::split(string, '|');
        if (stringVec.size() != 2) {
            return;
        }

        if (!StringUtil::isNumber(stringVec[1])) {
            return;
        }

        mod = stringVec[0];
        formID = std::stoi(stringVec[1]);
    }
}