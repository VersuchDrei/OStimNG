#pragma once

namespace OStim {
    struct ExpressionUpdater {
        int delay;
        int current;
        int goal;
        int speed;

        int step();
        bool isDone();
    };
}