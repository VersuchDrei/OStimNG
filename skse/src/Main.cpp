#include <stddef.h>

#include "Core.h"

#include "Core/ThreadInterface.h"
#include "Events/EventListener.h"
#include "GameLogic/GameHooks.h"
#include "GameLogic/GameTable.h"
#include "InterfaceSpec/IPluginInterface.h"
#include "InterfaceSpec/PluginInterface.h"
#include "Messaging/IMessages.h"
#include "Papyrus/Papyrus.h"
#include "PluginInterface/InterfaceExchangeMessage.h"
#include "PluginInterfaceImplementation/InterfaceMapImpl.h"
#include "Serial/Manager.h"
#include "UI/UIState.h"
#include "Util/RNGUtil.h"

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

namespace {
    void InitializeLogging() {
        auto path = log_directory();
        if (!path) {
            report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= PluginDeclaration::GetSingleton()->GetName();
        *path += L".log";

        std::shared_ptr<spdlog::logger> log;
        if (IsDebuggerPresent()) {
            log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
        } else {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
        }
        log->set_level(spdlog::level::info);
        log->flush_on(spdlog::level::info);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
    }

    void UnspecificedSenderMessageHandler(SKSE::MessagingInterface::Message* a_msg) {        
        switch (a_msg->type) {
            case OSAInterfaceExchangeMessage::kMessage_ExchangeInterface: {
                OSAInterfaceExchangeMessage* exchangeMessage = (OSAInterfaceExchangeMessage*)a_msg->data;
                exchangeMessage->interfaceMap = InterfaceMap::GetSingleton();
            } break;
            case OStim::InterfaceExchangeMessage::MESSAGE_TYPE: {
                OStim::InterfaceExchangeMessage* message = (OStim::InterfaceExchangeMessage*)a_msg->data;
                message->interfaceMap = Interface::InterfaceMapImpl::getSingleton();
            }
        }
    }

    void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {        
        switch (a_msg->type) {
            case SKSE::MessagingInterface::kPostLoad: {
                SKSE::GetNiNodeUpdateEventSource()->AddEventSink(Events::EventListener::GetSingleton());
                SKSE::GetCrosshairRefEventSource()->AddEventSink(Events::EventListener::GetSingleton());

                Core::postLoad();
            } break;
            case SKSE::MessagingInterface::kPostPostLoad: {
                Core::postpostLoad();

                // we are installing this hook so late because we need it to overwrite the PapyrusUtil hook
                GameLogic::installHooksPostPost();
            } break;
            case SKSE::MessagingInterface::kInputLoaded: {
                RE::BSInputDeviceManager::GetSingleton()->AddEventSink(Events::EventListener::GetSingleton());
            } break;
            case SKSE::MessagingInterface::kDataLoaded: {
                GameLogic::GameTable::setup();

                UI::PostRegisterMenus();

                Core::dataLoaded();
                SKSE::GetTaskInterface()->AddTask([]() { Core::postDataLoaded(); });
            } break;
            case SKSE::MessagingInterface::kPostLoadGame: {
                Events::EventListener::handleGameLoad();
                Core::newSession();
                Core::sessionStarted();
            } break;
            case SKSE::MessagingInterface::kNewGame: {
                Events::EventListener::handleGameLoad();
                Core::sessionLoaded();
                Core::sessionStarted();
            } break;

        }
    }
}  // namespace

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v;
    v.PluginVersion(REL::Version("7.3.5.3"sv));
    v.PluginName("OStim");
    v.AuthorName("VersuchDrei");
    v.UsesAddressLibrary(true);
    v.HasNoStructUse(true);

    return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info) {
    a_info->infoVersion = SKSE::PluginInfo::kVersion;
    a_info->name = "OStim";
    a_info->version = 0x07030053;

    return true;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const LoadInterface* skse) {
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);

    Init(skse);

    InterfaceMap::GetSingleton()->AddInterface("Messaging", Messaging::MessagingRegistry::GetSingleton());
    InterfaceMap::GetSingleton()->AddInterface("Threads", Interfaces::ThreadInterface::GetSingleton());

    auto message = SKSE::GetMessagingInterface();
    if (!message->RegisterListener(MessageHandler)) {
        return false;
    }
    if (!message->RegisterListener(nullptr, UnspecificedSenderMessageHandler)) {
        logger::warn("Plugin Interface wasn't initialized.");
    }

    Papyrus::Bind();
    UI::Settings::LoadSettings();

    GameLogic::installHooks();

    const auto serial = SKSE::GetSerializationInterface();
    serial->SetUniqueID(_byteswap_ulong('OST'));
    serial->SetSaveCallback(Serialization::Save);
    serial->SetLoadCallback(Serialization::Load);
    serial->SetRevertCallback(Serialization::Revert);

    UI::RegisterMenus();

    Core::init();
    
    log::info("{} has finished loading.", plugin->GetName());
    return true;
}