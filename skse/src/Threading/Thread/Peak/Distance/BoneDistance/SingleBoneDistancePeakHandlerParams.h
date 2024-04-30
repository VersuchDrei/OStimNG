#pragma once

#include "GameAPI/GameActorBone.h"

namespace Threading {
    namespace Thread {
        namespace Peak {
            struct SingleBoneDistancePeakHandlerParams {
                GameAPI::GameActorBone actorBone;
                GameAPI::GameActorBone targetBone;
            };
        }
    }
}