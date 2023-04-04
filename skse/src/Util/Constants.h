#pragma once

namespace Constants {
    inline std::default_random_engine RNG = std::default_random_engine{};

    constexpr int LOOP_TIME_MILLISECONDS = 20;
    constexpr float LOOP_TIME_SECONDS = 0.02;
}