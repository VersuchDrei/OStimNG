#pragma once

#include "InterfaceMap.h"

namespace OStim {
    struct InterfaceExchangeMessage {
        enum : uint32_t {
            kMessage_ExchangeInterface = 'OST'
        };

        InterfaceMap* interfaceMap = NULL;
    };
}