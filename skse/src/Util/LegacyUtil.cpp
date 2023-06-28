#include "LegacyUtil.h"

namespace LegacyUtil {
    std::string getIcon(Graph::Node* node) {
        if (!node) {
            return "logo";
        }

        if (node->hasNodeTag("missionary")) return "OStim/sexual/missionary_mf";
        if (node->hasNodeTag("cowgirl")) return "OStim/sexual/cowgirl_mf";
        if (node->hasNodeTag("reversecowgirl")) return "OStim/sexual/reversecowgirl_mf";
        if (node->hasNodeTag("doggystyle")) return "OStim/sexual/doggystyle_mf";
        if (node->hasNodeTag("sixtynine")) return "OStim/sexual/sixtynine_mf";

        if (node->findAction("vaginalsex") != -1) return "OStim/sexual/vaginalsex_mf";
        if (node->findAction("analsex") != -1) return "OStim/sexual/analsex_mf";
        if (node->findAction("blowjob") != -1) return "OStim/sexual/blowjob_mf";
        if (node->findAction("cunnilingus") != -1) return "OStim/sexual/cunnilingus_mf";
        if (node->findAction("handjob") != -1) return "OStim/sexual/handjob_mf";
        if (node->findAction("vaginalfisting") != -1) return "OStim/sexual/vaginalfisting_mf";
        if (node->findAction("vaginalfingering") != -1) return "OStim/sexual/vaginalfingering_mf";
        if (node->findAction("vampirebite") != -1) return "OStim/sensual/vampirebite";
        if (node->findAction("kissing") != -1) return "OStim/sensual/kissing_mf";
        if (node->findAction("kissingneck") != -1) return "OStim/sensual/kissingneck_mf";
        if (node->findAction("licking") != -1) return "OStim/sensual/licking";
        if (node->findAction("gropingbutt") != -1) return "OStim/detail/gropingbutt_mf";
        if (node->findAction("holdinghead") != -1) return "OStim/detail/holdinghead_mf";
        if (node->findAction("holdinghip") != -1) return "OStim/detail/holdinghip_mf";

        return "placeholder";
    }
}