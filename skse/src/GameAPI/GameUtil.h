#pragma once

namespace GameAPI {
    namespace GameUtil {
        void forEachReferenceInRange(const RE::NiPoint3& center, float radius, std::function<RE::BSContainer::ForEachResult(RE::TESObjectREFR&)> callback);
    }
}