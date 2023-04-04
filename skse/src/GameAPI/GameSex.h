#pragma once

namespace GameAPI {
    enum GameSex { MALE, FEMALE, NONE };

    class GameSexAPI {
    public:
        static GameSex fromString(std::string sex);
        static std::string toIcon(GameSex sex);
    };
}  // namespace GameAPI