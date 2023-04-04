#pragma once
#include <Messaging/IMessages.h>

namespace Messaging {
    void MessagingRegistry::RegisterForMessages(std::string newListener) {
        logger::info("registering {}", newListener);
        if (std::find(registeredListeners.begin(), registeredListeners.end(), newListener) ==
            registeredListeners.end()) {
            registeredListeners.push_back(newListener);
        }
    }
}  // namespace Messaging