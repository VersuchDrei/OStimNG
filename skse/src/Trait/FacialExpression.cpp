#include "FacialExpression.h"

#include "TraitTable.h"

namespace Trait {
    GenderExpression* FacialExpression::getGenderExpression(bool isFemale) {
        return isFemale ? &female : &male;
    }

    float FacialExpression::getDuration(GameAPI::GameActor actor) {
        if (actor.isSex(GameAPI::GameSex::FEMALE)) {
            return female.duration;
        } else {
            return male.duration;
        }
    }
}