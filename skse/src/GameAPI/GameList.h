#pragma once

#include "GameObject.h"
#include "GameRecord.h"

namespace GameAPI {
    struct GameList : public GameRecord<RE::BGSListForm> {
    public:
        inline GameList() {}
        inline GameList(RE::BGSListForm* list) { form = list; }

        inline bool contains(GameObject object) const { return form->HasForm(object.form->GetBaseObject()->formID); }
    };
}