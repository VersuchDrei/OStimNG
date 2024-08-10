#pragma once

#include "Thread.h"
#include "ThreadActorEventListener.h"
#include "ThreadEventListener.h"

#include "../PluginInterface.h"

namespace OStim {
    class ThreadInterface : public PluginInterface {
    public:
        inline static const char* NAME = "Threads";

        // --- API version 1 ---

        /**
        * gets the thread with the given ID
        * returns nullptr if no thread with the ID is running
        */
        virtual Thread* getThread(int32_t threadID) = 0;

        virtual void registerThreadStartListener(ThreadEventListener* listener) = 0;
        virtual void registerSpeedChangedListener(ThreadEventListener* listener) = 0;
        virtual void registerNodeChangedListener(ThreadEventListener* listener) = 0;
        virtual void registerClimaxListener(ThreadActorEventListener* listener) = 0;
        virtual void registerThreadStopListener(ThreadEventListener* listener) = 0;
    };
}