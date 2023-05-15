#pragma once

#include "SoundPlayer.h"

#include "GameAPI/GameSound.h"

// this is required because C++ can't do circular references in header files
namespace OStim {
    class ThreadActor;
}

namespace Sound {
    class SoundType {
    public:
        static SoundType* fromJson(std::string path, json json);

        inline SoundType(GameAPI::GameSound sound) : sound{sound} {}

        virtual SoundPlayer* create(OStim::ThreadActor* actor, OStim::ThreadActor* target) = 0;

    protected:
        GameAPI::GameSound sound;
    };
}