#pragma once

#include "ToyGroup.h"

#include "Core/Thread.h"

namespace Toys {
    class ToyThread {
    public:
        ToyThread(OStim::Thread* thread);

    private:
        OStim::Thread* thread;
        std::vector<ToyGroup> groups;


        void loop();
        void peak(actionIndex action);
        void speedChanged();
        void nodeChanged();
        void threadEnd();
    };
}