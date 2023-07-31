#pragma once

#include "GameActor.h"
#include "GameRecord.h"

namespace GameAPI {
    struct GameCondition : public GameRecord<RE::BGSPerk> {
    public:
        inline GameCondition() {}
        inline GameCondition(RE::BGSPerk* perk) { form = perk; }

        inline int getPriority() { return form->data.level; }
        inline bool fulfills(GameActor actor) { return form->perkConditions.IsTrue(actor.form, actor.form); }
    };
}