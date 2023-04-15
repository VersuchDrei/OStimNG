#include "GameSex.h"

#include "Util/StringUtil.h"

namespace GameAPI {
    GameSex GameSexAPI::fromString(std::string sex) {
        StringUtil::toLower(&sex);

        if (sex == "male") {
            return GameSex::MALE;
        } else if (sex == "female") {
            return GameSex::FEMALE;
        }

        return GameSex::NONE;
    }

    GameSex GameSexAPI::fromGame(RE::SEX sex) {
        switch (sex) {
            case RE::SEX::kNone:
                return GameSex::NONE;
            case RE::SEX::kMale:
                return GameSex::MALE;
            case RE::SEX::kFemale:
                return GameSex::FEMALE;
            default:
                return GameSex::NONE;
        }
    }

    std::string GameSexAPI::toIcon(GameSex sex) {
        switch (sex) {
            case NONE:
                return "N";
            case MALE:
                return "M";
            case FEMALE:
                return "F";
            default:
                return "U";
        }
    }
}  // namespace GameAPI