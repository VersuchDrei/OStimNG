#include "PeakHandler.h"

#include "Threading/Thread/NodeHandler.h"

namespace Threading {
    namespace Thread {
        namespace Peak {
            PeakHandler::PeakHandler(PeakHandlerParams params) : peakListener{params.peakListener} {
                params.nodeHandler->registerLoopListener([this]() { loop(); });
                params.nodeHandler->registerNodeChangedListener([this]() { nodeChanged(); });
                params.nodeHandler->registerThreadEndListener([this]() { threadEnd(); });
            }
        }
    }
}