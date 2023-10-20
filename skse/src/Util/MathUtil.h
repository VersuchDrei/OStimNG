#pragma once

#include "Util/Constants.h"

#define _USE_MATH_DEFINES

#include <math.h>

namespace MathUtil {
    inline float toDegrees(float angle) { return angle * 180 / M_PI; }
    inline float toRadians(float angle) { return angle * M_PI / 180; }
    inline bool chanceRoll(int chance) { return chance > std::uniform_int_distribution<>(0, 99)(Constants::RNG); }
    inline int randomInt(int min, int max) { return std::uniform_int_distribution<>(min, max)(Constants::RNG); }
    int32_t uintToInt(uint32_t value);
    uint32_t intToUint(int32_t value);
}