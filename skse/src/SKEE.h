#pragma once

namespace SKEE {
    class IPluginInterface {
    public:
        IPluginInterface(){};
        virtual ~IPluginInterface(){};

        virtual uint32_t GetVersion() = 0;
        virtual void Revert() = 0;
    };

    class IInterfaceMap {
    public:
        virtual IPluginInterface* QueryInterface(const char* name) = 0;
        virtual bool AddInterface(const char* name, IPluginInterface* pluginInterface) = 0;
        virtual IPluginInterface* RemoveInterface(const char* name) = 0;
    };

    struct InterfaceExchangeMessage {
        enum : uint32_t { kExchangeInterface = 0x9E3779B9 };

        IInterfaceMap* interfaceMap = nullptr;
    };

    class IAddonAttachmentInterface {
    public:
        virtual void OnAttach(RE::TESObjectREFR* refr, RE::TESObjectARMO* armor, RE::TESObjectARMA* addon, RE::NiAVObject* object, bool isFirstPerson, RE::NiNode* skeleton, RE::NiNode* root) = 0;
    };

    class INiTransformInterface : public IPluginInterface {
    public:
        struct Position {
            float x, y, z;
        };
        struct Rotation {
            float heading, attitude, bank;
        };

        // Visits all overrides within a set
        class NodeVisitor {
        public:
            virtual bool VisitPosition(const char* node, const char* key, Position& position) = 0;
            virtual bool VisitRotation(const char* node, const char* key, Rotation& rotation) = 0;
            virtual bool VisitScale(const char* node, const char* key, float scale) = 0;
            virtual bool VisitScaleMode(const char* node, const char* key, uint32_t scaleMode) = 0;
        };

        virtual bool HasNodeTransformPosition(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual bool HasNodeTransformRotation(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual bool HasNodeTransformScale(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual bool HasNodeTransformScaleMode(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;

        virtual void AddNodeTransformPosition(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name, Position& position) = 0;  // X,Y,Z
        virtual void AddNodeTransformRotation(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name, Rotation& rotation) = 0;  // Euler angles
        virtual void AddNodeTransformScale(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name, float scale) = 0;
        virtual void AddNodeTransformScaleMode(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name, uint32_t scaleMode) = 0;

        virtual Position GetNodeTransformPosition(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual Rotation GetNodeTransformRotation(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual float GetNodeTransformScale(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual uint32_t GetNodeTransformScaleMode(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;

        virtual bool RemoveNodeTransformPosition(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual bool RemoveNodeTransformRotation(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual bool RemoveNodeTransformScale(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual bool RemoveNodeTransformScaleMode(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;

        virtual bool RemoveNodeTransform(RE::TESObjectREFR* refr, bool firstPerson, bool isFemale, const char* node, const char* name) = 0;
        virtual void RemoveAllReferenceTransforms(RE::TESObjectREFR* refr) = 0;

        virtual bool GetOverrideNodeTransform(RE::TESObjectREFR* refr, bool firstPerson, bool isFemale, const char* node, const char* name, uint16_t key, RE::NiTransform* result) = 0;

        virtual void UpdateNodeAllTransforms(RE::TESObjectREFR* ref) = 0;

        virtual void VisitNodes(RE::TESObjectREFR* refr, bool firstPerson, bool isFemale, NodeVisitor& visitor) = 0;
        virtual void UpdateNodeTransforms(RE::TESObjectREFR* ref, bool firstPerson, bool isFemale, const char* node) = 0;
    };

    class IActorUpdateManager : public IPluginInterface {
    public:
        virtual void AddBodyUpdate(RE::FormID formId) = 0;
        virtual void AddTransformUpdate(RE::FormID formId) = 0;
        virtual void AddOverlayUpdate(RE::FormID formId) = 0;
        virtual void AddNodeOverrideUpdate(RE::FormID formId) = 0;
        virtual void AddWeaponOverrideUpdate(RE::FormID formId) = 0;
        virtual void AddAddonOverrideUpdate(RE::FormID formId) = 0;
        virtual void AddSkinOverrideUpdate(RE::FormID formId) = 0;
        virtual void Flush() = 0;
        virtual void AddInterface(IAddonAttachmentInterface* observer) = 0;
        virtual void RemoveInterface(IAddonAttachmentInterface* observer) = 0;
    };
}