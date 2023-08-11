#pragma once

namespace GameAPI {
    struct GamePosition {
    public:
        float x = 0;
        float y = 0;
        float z = 0;
        float r = 0;

        inline GamePosition() {}
        inline GamePosition(RE::NiPoint3 position, float r) {
            x = position.x;
            y = position.y;
            z = position.z;
            this->r = r;
        }
        inline GamePosition(float x, float y, float z, float r) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->r = r;
        }
    };
}