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