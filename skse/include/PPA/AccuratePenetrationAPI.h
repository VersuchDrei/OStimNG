#pragma once

#include <cstdint>

#include "RE/B/BSPointerHandle.h"

#ifndef ACCURATEPENETRATION_API
    #define ACCURATEPENETRATION_API
#endif

namespace AccuratePenetration::API {
    inline constexpr auto kPluginDLL = L"AccuratePenetration.dll";
    inline constexpr auto kGetAPIFunctionNameV1 = "AccuratePenetration_GetAPI_V1";
    inline constexpr auto kSetActorInteractionFunctionName = "AccuratePenetration_SetActorInteraction";
    inline constexpr std::uint32_t kVersion = 1;

    enum class SceneContext : std::uint32_t {
        None = 0,
        Vaginal = 1 << 0,
        Anal = 1 << 1,
        Oral = 1 << 2,
        Aggressive = 1 << 3,
        FemDom = 1 << 4,
        Loving = 1 << 5,
        Dirty = 1 << 6,
        Boobjob = 1 << 7,
        Handjob = 1 << 8,
        Footjob = 1 << 9,
        Masturbation = 1 << 10,
    };

    [[nodiscard]] constexpr SceneContext operator|(SceneContext a_lhs, SceneContext a_rhs) noexcept { return static_cast<SceneContext>(static_cast<std::uint32_t>(a_lhs) | static_cast<std::uint32_t>(a_rhs)); }

    [[nodiscard]] constexpr SceneContext operator&(SceneContext a_lhs, SceneContext a_rhs) noexcept { return static_cast<SceneContext>(static_cast<std::uint32_t>(a_lhs) & static_cast<std::uint32_t>(a_rhs)); }

    [[nodiscard]] constexpr bool HasContext(SceneContext a_flags, SceneContext a_check) noexcept { return (a_flags & a_check) == a_check; }

    enum class PenetrationSite : std::uint8_t {
        None = 0,
        Mouth,
        Anus,
        Vagina,
        Both,
        HandL,
        HandR,
        Hands,
    };

    // Auto = PPA's heuristic choice, None = explicitly disabled. Numeric values mirror SexScene::ActionTarget.
    enum class InteractionAction : std::uint8_t {
        Auto = 0,
        None,
        Vagina,
        Anus,
        Mouth,
        Hands,
        HandL,
        HandR,
    };

    struct InteractionPartner {
        RE::ActorHandle actor;
        PenetrationSite site = PenetrationSite::None;
        std::uint8_t position = 0;
        float penetrationDepth = 0.0f;
        float penisSize = 0.0f;
        float penisGirth = 0.0f;
    };

    struct AnimationUpdateEvent {
        std::uint32_t apiVersion = kVersion;
        std::uint32_t size = sizeof(AnimationUpdateEvent);

        RE::ActorHandle receiver;
        std::uint8_t position = 1;
        SceneContext context = SceneContext::None;

        const InteractionPartner* selfInteraction = nullptr;
        const InteractionPartner* actors = nullptr;
        std::uint32_t actorCount = 0;

        float anusOpening = 0.0f;
        float vaginalOpening = 0.0f;
        // True for the final cleanup notification for this receiver's scene.
        bool ending = false;
    };

    using ListenerHandle = std::uint64_t;
    using AnimationUpdateCallback = void(__cdecl*)(const AnimationUpdateEvent* a_event, void* a_userData);
    using RegisterAnimationUpdateListenerFn = ListenerHandle(__cdecl*)(AnimationUpdateCallback a_callback, void* a_userData);
    using UnregisterAnimationUpdateListenerFn = bool(__cdecl*)(ListenerHandle a_handle);
    // Sets what a_performer does in its running scene for the current stage, like the target menu but not saved.
    // Empty a_receiver = any receiver; a_performer == a_receiver = self-interaction. Overwritten by tag reloads and animation changes.
    // Returns false if PPA is not tracking the performer's (and receiver's) scene yet.
    using SetActorInteractionFn = bool(__cdecl*)(RE::ActorHandle a_performer, InteractionAction a_action, RE::ActorHandle a_receiver);

    struct InterfaceV1 {
        std::uint32_t version = kVersion;
        std::uint32_t size = sizeof(InterfaceV1);

        RegisterAnimationUpdateListenerFn RegisterAnimationUpdateListener = nullptr;
        UnregisterAnimationUpdateListenerFn UnregisterAnimationUpdateListener = nullptr;
    };

    // Main-thread only. Register/unregister on Skyrim's main thread. Callbacks run during this plugin's main-thread update and cleanup.
    // AnimationUpdateEvent and nested pointers are temporary; copy any data you need before returning from the callback.
}  // namespace AccuratePenetration::API

extern "C" ACCURATEPENETRATION_API const AccuratePenetration::API::InterfaceV1* __cdecl AccuratePenetration_GetAPI_V1();
extern "C" ACCURATEPENETRATION_API bool __cdecl AccuratePenetration_SetActorInteraction(RE::ActorHandle a_performer, AccuratePenetration::API::InteractionAction a_action, RE::ActorHandle a_receiver);