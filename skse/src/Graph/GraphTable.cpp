#include "Graph/GraphTable.h"

namespace Graph {    
    Requirement GraphTable::getRequirement(std::string string) {
        auto iter = requirements.find(string);
        if (iter != requirements.end()) {
            return iter->second;
        }

        return Requirement::NONE;
    }
}