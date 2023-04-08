#pragma once

namespace ExtraDataUtil {
    inline void setOwnerForm(RE::ExtraDataList* list, RE::TESForm* owner) {
        using func_t = decltype(setOwnerForm);
        REL::Relocation<func_t> func{RELOCATION_ID(11463, 11609)};
        func(list, owner);
    }
}