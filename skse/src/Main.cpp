#include <stddef.h>

#include "Core.h"

#include "Core/ThreadInterface.h"
#include "Events/EventListener.h"
#include "GameAPI/GameHooks.h"
#include "GameAPI/GameTable.h"
#include "InterfaceSpec/IPluginInterface.h"
#include "InterfaceSpec/PluginInterface.h"
#include "Messaging/IMessages.h"
#include "Papyrus/Papyrus.h"
#include "Serial/Manager.h"
#include "UI/UIState.h"
#include "Util/RNGUtil.h"

using namespace RE::BSScript;
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
        }
    }

    void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {        
        switch (a_msg->type) {
            case SKSE::MessagingInterface::kPostLoad: {
                RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink<RE::TESLoadGameEvent>(Events::EventListener::GetSingleton());
                SKSE::GetNiNodeUpdateEventSource()->AddEventSink(Events::EventListener::GetSingleton());
                SKSE::GetCrosshairRefEventSource()->AddEventSink(Events::EventListener::GetSingleton());

                auto message = SKSE::GetMessagingInterface();
                if (message) {
                    message->RegisterListener(nullptr, UnspecificedSenderMessageHandler);
                }

                Core::postLoad();
            } break;
            case SKSE::MessagingInterface::kPostPostLoad: {
                Core::postpostLoad();
            } break;
            case SKSE::MessagingInterface::kInputLoaded: {
                RE::BSInputDeviceManager::GetSingleton()->AddEventSink(Events::EventListener::GetSingleton());
            } break;
            case SKSE::MessagingInterface::kDataLoaded: {
                GameAPI::GameTable::setup();

                UI::PostRegisterMenus();
                
                // we are installing this hook so late because we need it to overwrite the PapyrusUtil hook
                GameAPI::installHooksLate();

                Core::dataLoaded();
                SKSE::GetTaskInterface()->AddTask([]() { Core::postDataLoaded(); });
            } break;
            case SKSE::MessagingInterface::kPreLoadGame: {
                //UI::PostRegisterMenus();
            } break;
            case SKSE::MessagingInterface::kNewGame: {
                Events::EventListener::handleGameLoad();
                //UI::PostRegisterMenus();
            } break;

        }
    }
}  // namespace

SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLogging();

    auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is loading...", plugin->GetName(), version);

    Init(skse);

    Core::init();

    InterfaceMap::GetSingleton()->AddInterface("Messaging", Messaging::MessagingRegistry::GetSingleton());
    InterfaceMap::GetSingleton()->AddInterface("Threads", Interfaces::ThreadInterface::GetSingleton());

    auto message = SKSE::GetMessagingInterface();
    if (!message->RegisterListener(MessageHandler)) {
        return false;
    }

    Papyrus::Bind();
    UI::Settings::LoadSettings();

    GameAPI::installHooks();

    const auto serial = SKSE::GetSerializationInterface();
    serial->SetUniqueID(_byteswap_ulong('OST'));
    serial->SetSaveCallback(Serialization::Save);
    serial->SetLoadCallback(Serialization::Load);
    serial->SetRevertCallback(Serialization::Revert);

    UI::RegisterMenus();

    RNGUtil::setup();
    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
