#include "FacialExpression.h"

#include "TraitTable.h"

namespace Trait {
    GenderExpression* FacialExpression::getGenderExpression(bool isFemale) {
        return isFemale ? &female : &male;
    }

    float FacialExpression::getDuration(RE::Actor* actor) {
        if (actor->GetActorBase()->GetSex() == RE::SEX::kFemale) {
            return female.duration;
        } else {
            return male.duration;
        }
    }
}