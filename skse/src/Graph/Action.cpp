#include "Action.h"

#include "Util/VectorUtil.h"

namespace Graph {
    bool ActionAttributes::hasTag(std::string tag) {
        return VectorUtil::contains(tags, tag);
    }

    bool Action::doFullStrip(int position) {
        return actor == position && attributes->actor.fullStrip || 
              target == position && attributes->target.fullStrip || 
           performer == position && attributes->performer.fullStrip;
    }

    uint32_t Action::getStrippingMask(int position) {
        return (actor == position ? attributes->actor.strippingMask : 0) |
               (target == position ? attributes->target.strippingMask : 0) |
               (performer == position ? attributes->performer.strippingMask : 0);
    }
}