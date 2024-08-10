#pragma once

#include "Thread.h"

namespace OStim {
    class ThreadEventListener {
    public:
        virtual void listen(Thread* thread) = 0;
    };
}