#pragma once

#include "PeakHandlerParams.h"

namespace Threading {
    namespace Threads {
        namespace Peak {
            class PeakHandler {
            public:
                PeakHandler(PeakHandlerParams params);

            protected:
                virtual void loop() = 0;
                inline void peak() { peakListener(); };

            private:
                std::function<void()> peakListener;

                inline void nodeChanged() { delete this; }
                inline void threadEnd() { delete this; }
            };
        }  // namespace Peak
    }}