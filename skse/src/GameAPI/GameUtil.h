#pragma once

namespace GameAPI {
    namespace GameUtil {
        void sendModEvent(RE::TESForm* sender, std::string eventName, std::string strArg, float numArg);

        void forEachReferenceInRange(const RE::NiPoint3& center, float radius, std::function<RE::BSContainer::ForEachResult(RE::TESObjectREFR&)> callback);

        // bandaid function until CLib-NG is updated
        void CompileAndRun(RE::Script* script, RE::TESObjectREFR* targetRef, RE::COMPILER_NAME name = RE::COMPILER_NAME::kSystemWindowCompiler);

        // bandaid function until CLib-NG is updated
        void ForEachReferenceInRange(RE::TESObjectREFR* a_origin, float a_radius, std::function<RE::BSContainer::ForEachResult(RE::TESObjectREFR& a_ref)> a_callback);

        inline void CompileAndRunImpl(RE::Script* script, RE::ScriptCompiler* compiler, RE::COMPILER_NAME name, RE::TESObjectREFR* targetRef) {
			using func_t = decltype(CompileAndRunImpl);
			REL::Relocation<func_t> func{ RELOCATION_ID(21416, REL::Module::get().version().patch() < 1130 ? 21890 : 441582) };
			return func(script, compiler, name, targetRef);
        }
    }
}