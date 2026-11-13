#include "GenitalAlignmentAPI.h"

#include "Util/StringUtil.h"

namespace Interface {
    namespace GenitalAlignmentAPI {
        OStim::GenitalAlignment fromString(std::string action) {
            StringUtil::toLower(&action);

            if (action == "vagina") {
                return OStim::GenitalAlignment::VAGINA;
            } else if (action == "anus") {
                return OStim::GenitalAlignment::ANUS;
            } else if (action == "mouth") {
                return OStim::GenitalAlignment::MOUTH;
            } else if (action == "hands") {
                return OStim::GenitalAlignment::HANDS;
            } else if (action == "handr") {
                return OStim::GenitalAlignment::HAND_R;
            } else if (action == "handl") {
                return OStim::GenitalAlignment::HAND_L;
            } else if (action == "none") {
                return OStim::GenitalAlignment::NONE;
            }

            return OStim::GenitalAlignment::UNDEFINED;
        }
    }
}