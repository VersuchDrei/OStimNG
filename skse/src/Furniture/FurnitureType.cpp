#include "FurnitureType.h"

namespace Furniture{
    FurnitureType* FurnitureType::getMasterType() {
        if (supertype) {
            return supertype->getMasterType();
        }
        return this;
    }

    FurnitureType* FurnitureType::getListType() {
        if (listIndividually){
            return this;
        }
        if (supertype) {
            return supertype->getListType();
        }
        return this;
    }

    bool FurnitureType::isChildOf(FurnitureType* other) {
        if (this == other) {
            return true;
        }
        if (supertype) {
            return supertype->isChildOf(other);
        }
        return false;
    }
}