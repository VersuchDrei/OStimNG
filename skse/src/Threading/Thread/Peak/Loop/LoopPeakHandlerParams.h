#pragma once

namespace Threading {
    namespace Thread {
        namespace Peak {
            struct LoopPeakHandlerParams {
                int interval = 1000;
                bool scaleWithSpeed = true;
            };
        }
    }
}