/*
* Generic OStim Thread Data API
* For any mod that needs to access running thread/scene data
* This is NOT specific to any particular UI implementation
*/
#pragma once

namespace OstimNG_API::Thread
{
    enum class InterfaceVersion : uint8_t
    {
        V1
    };

    enum class APIResult : uint8_t
    {
        OK,
        Invalid,
        Failed
    };

    // Event types
    enum class ThreadEvent : uint8_t
    {
        ThreadStarted,
        ThreadEnded,
        NodeChanged,
        ControlInput
    };

    // Control input types
    enum class Controls : uint8_t
    {
        Up,
        Down,
        Left,
        Right,
        Toggle,
        Yes,
        No,
        Menu,
        KEY_HIDE,
        AlignMenu,
        SearchMenu
    };

    // Key mappings
    struct KeyData
    {
        int keyUp;
        int keyDown;
        int keyLeft;
        int keyRight;
        int keyYes;
        int keyEnd;
        int keyToggle;
        int keySearch;
        int keyAlignment;
        int keySceneStart;
        int keyNpcSceneStart;
        int keySpeedUp;
        int keySpeedDown;
        int keyPullOut;
        int keyAutoMode;
        int keyFreeCam;
        int keyHideUI;
    };

    // Event callback function type
    typedef void (*ThreadEventCallback)(ThreadEvent eventType, uint32_t threadID, void* userData);

    // Control event callback function type
    typedef void (*ControlEventCallback)(Controls controlType, uint32_t threadID, void* userData);

    // Actor data in a scene
    struct ActorData
    {
        uint32_t formID;           // Actor FormID
        float excitement;          // 0.0 to 100.0
        bool isFemale;
        bool hasSchlong;
        int timesClimaxed;         // Number of times the actor has climaxed in this scene
    };

    // Navigation option from current node
    struct NavigationData
    {
        std::string sceneId;
        std::string destinationId;  // Final destination scene ID (after transitions)
        std::string icon;
        std::string description;
        std::string border;         // Border color hex (e.g. "ffffff")
        bool isTransition;          // Whether this navigation goes through a transition
    };

    // Actor alignment/positioning data
    struct ActorAlignmentData
    {
        float offsetX;
        float offsetY;
        float offsetZ;
        float scale;
        float rotation;
        float sosBend;
    };

    // Scene search result
    struct SceneSearchResult
    {
        std::string sceneId;
        std::string name;
        uint32_t actorCount;
    };

    // Options menu item (sub-page or executable option on the current page)
    struct OptionsMenuItem
    {
        std::string id;
        std::string title;
        std::string icon;
        std::string border;
        std::string description;
    };

    // Generic thread data access interface
    class IThreadInterface
    {
    public:
        // Get player's current thread ID (0 if not in scene)
        virtual uint32_t GetPlayerThreadID() noexcept = 0;

        // Check if thread exists and is valid
        virtual bool IsThreadValid(uint32_t threadID) noexcept = 0;

        // Get current scene ID for thread (nullptr if invalid)
        virtual const char* GetCurrentSceneID(uint32_t threadID) noexcept = 0;

        // Get actor count in thread
        virtual uint32_t GetActorCount(uint32_t threadID) noexcept = 0;

        // Get actor data for thread
        // buffer: array to fill with actor data
        // bufferSize: size of buffer array
        // Returns: number of actors filled (0 if invalid thread)
        virtual uint32_t GetActors(uint32_t threadID, ActorData* buffer, uint32_t bufferSize) noexcept = 0;

        // Get navigation option count for current node
        virtual uint32_t GetNavigationCount(uint32_t threadID) noexcept = 0;

        // Get navigation options for current node
        // buffer: array to fill with navigation data
        // bufferSize: size of buffer array
        // Returns: number of options filled (0 if invalid/no options)
        virtual uint32_t GetNavigationOptions(uint32_t threadID, NavigationData* buffer, uint32_t bufferSize) noexcept = 0;

        // Navigate to a different scene
        virtual APIResult NavigateToScene(uint32_t threadID, const char* sceneID) noexcept = 0;

        // Check if node is transition
        virtual bool IsTransition(uint32_t threadID) noexcept = 0;

        // Check if thread is in sequence
        virtual bool IsInSequence(uint32_t threadID) noexcept = 0;

        // Check if thread is in auto mode (player-toggled automatic progression)
        virtual bool IsAutoMode(uint32_t threadID) noexcept = 0;

        // Check if player control is disabled
        virtual bool IsPlayerControlDisabled(uint32_t threadID) noexcept = 0;

        // Register callback for thread events
        virtual void RegisterEventCallback(ThreadEventCallback callback, void* userData) noexcept = 0;

        // Unregister callback
        virtual void UnregisterEventCallback(ThreadEventCallback callback) noexcept = 0;

        // Register callback for control input events
        virtual void RegisterControlCallback(ControlEventCallback callback, void* userData) noexcept = 0;

        // Unregister control callback
        virtual void UnregisterControlCallback(ControlEventCallback callback) noexcept = 0;

        // Enable/disable external UI (prevents OStim's flash menus from showing)
        virtual void SetExternalUIEnabled(bool enabled) noexcept = 0;

        // Get configured hotkeys
        virtual void GetKeyData(KeyData* outData) noexcept = 0;

        // --- Navigation ---

        // Get display name of the current scene/node (nullptr if invalid)
        virtual const char* GetCurrentNodeName(uint32_t threadID) noexcept = 0;

        // Get current animation speed index (0 if invalid)
        virtual int32_t GetCurrentSpeed(uint32_t threadID) noexcept = 0;

        // Get max speed index for current node (0 if invalid)
        virtual int32_t GetMaxSpeed(uint32_t threadID) noexcept = 0;

        // Set animation speed by index
        virtual APIResult SetSpeed(uint32_t threadID, int32_t speed) noexcept = 0;

        // --- Alignment ---

        // Get actor alignment/positioning for a given actor slot
        // Returns false if invalid thread or actor index
        virtual bool GetActorAlignment(uint32_t threadID, uint32_t actorIndex, ActorAlignmentData* outData) noexcept = 0;

        // Set actor alignment/positioning for a given actor slot
        virtual APIResult SetActorAlignment(uint32_t threadID, uint32_t actorIndex, const ActorAlignmentData* data) noexcept = 0;

        // --- Search ---

        // Search scenes by name substring (case-insensitive)
        // buffer: array to fill with search results
        // bufferSize: max results to return
        // Returns: number of results filled
        virtual uint32_t SearchScenes(const char* query, SceneSearchResult* buffer, uint32_t bufferSize) noexcept = 0;

        // Get info about a specific scene by ID
        // Returns false if scene not found
        virtual bool GetSceneInfo(const char* sceneID, SceneSearchResult* outInfo) noexcept = 0;

        // Navigate to a scene found via search (validates the scene exists before navigating)
        virtual APIResult NavigateToSearchResult(uint32_t threadID, const char* sceneID) noexcept = 0;

        // --- Options Menu ---

        // Rebuild the options page tree (call when node changes)
        virtual void RebuildOptionsTree() noexcept = 0;

        // Get the number of items on the current options page
        // This count does NOT include the back/exit item (index -1)
        virtual uint32_t GetOptionsItemCount() noexcept = 0;

        // Get items for the current options page (sub-pages followed by options)
        // buffer: array to fill, bufferSize: max items
        // Returns: number of items filled (does NOT include back/exit)
        virtual uint32_t GetOptionsItems(OptionsMenuItem* buffer, uint32_t bufferSize) noexcept = 0;

        // Select an item on the current options page
        // index -1: go back (or exit if at root)
        // index 0+: select the item at that position (sub-page or option)
        // Returns true if the options menu is still open, false if it exited (back from root)
        virtual bool SelectOptionsItem(int32_t index) noexcept = 0;

        // Check if currently at the root level of the options menu
        virtual bool IsOptionsAtRoot() noexcept = 0;
    };

    using _RequestPluginAPI_Thread = IThreadInterface* (*)(InterfaceVersion a_interfaceVersion, const char* a_pluginName, REL::Version a_pluginVersion);

    inline IThreadInterface* GetAPI(const char* pluginName, REL::Version pluginVersion)
    {
        const auto ostim = GetModuleHandleA("OStim.dll");
        if (!ostim)
            return nullptr;

        const auto requestAPI = reinterpret_cast<_RequestPluginAPI_Thread>(
            reinterpret_cast<void*>(GetProcAddress(ostim, "RequestPluginAPI_Thread")));
        if (!requestAPI)
            return nullptr;

        return requestAPI(InterfaceVersion::V1, pluginName, pluginVersion);
    }
    
    // Internal helper for OStim code to notify events
    // External mods should use RegisterEventCallback instead
    void NotifyEvent(ThreadEvent eventType, uint32_t threadID);
    
    // Internal helper for OStim code to notify control input
    // External mods should use RegisterControlCallback instead
    void NotifyControlInput(Controls controlType, uint32_t threadID);
}

extern OstimNG_API::Thread::IThreadInterface* g_ostimThreadInterface;
