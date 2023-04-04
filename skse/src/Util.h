#pragma once

namespace stl {
    std::vector<std::string> string_split(const std::string& text, char sep);
}

namespace util {
    std::optional<fs::path> documents_path();
    std::optional<fs::path> database_path();
    std::optional<fs::path> settings_path();

    bool within_radius(const RE::NiPoint3& a_origin, const RE::NiPoint3& a_obj, float a_radius);

    void iterate_attached_cells(const RE::NiPoint3& a_origin, float a_radius,
                                std::function<RE::BSContainer::ForEachResult(RE::TESObjectREFR&)> a_fn);
}  // namespace util