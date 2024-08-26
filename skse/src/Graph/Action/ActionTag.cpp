#include "ActionTag.h"

namespace Graph {
    namespace Action {
        const char* ActionTag::getTagID() {
            return tag.c_str();
        }
    }
}