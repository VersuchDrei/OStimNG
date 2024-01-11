#include "Graph/GraphTable.h"

#include "Util/StringUtil.h"

namespace Graph {    
    Requirement GraphTable::getRequirement(std::string string) {
        StringUtil::toLower(&string);
        auto iter = requirements.find(string);
        if (iter != requirements.end()) {
            return iter->second;
        }

        return Requirement::NONE;
    }
}