#pragma once

namespace GameAPI {
    struct GamePosition {
    public:
        float x = 0;
        float y = 0;
        float z = 0;

        inline GamePosition() {}
        inline GamePosition(RE::NiPoint3 position) {
            x = position.x;
            y = position.y;
            z = position.z;
        }
    };
}