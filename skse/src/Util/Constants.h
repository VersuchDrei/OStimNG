#pragma once

namespace Constants {
    inline std::default_random_engine RNG = std::default_random_engine{};
    inline std::regex hexColor("^[a-f0-9]{6}$");

    constexpr int LOOP_TIME_MILLISECONDS = 20;
    constexpr float LOOP_TIME_SECONDS = 0.02;
}