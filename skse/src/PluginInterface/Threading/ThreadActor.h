#pragma once

namespace OStim {
    class ThreadActor {
    public:
        // cast this to RE::Actor*
        virtual void* getGameActor() = 0;
    };
}