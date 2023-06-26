#include <stddef.h>

#include "Alignment/Alignments.h"
#include "Events/EventListener.h"
#include "Furniture/FurnitureTable.h"
#include "Game/Patch.h"
#include "GameAPI/GameTable.h"
#include "Graph/GraphTable.h"
#include "InterfaceSpec/IPluginInterface.h"
#include "InterfaceSpec/PluginInterface.h"
#include "Messaging/IMessages.h"
#include "Papyrus/Papyrus.h"
#include "Serial/Manager.h"
#include "Sound/SoundTable.h"
#include "Trait/TraitTable.h"
#include "UI/Align/AlignMenu.h"
#include "Util/CompatibilityTable.h"
#include "Util/LookupTable.h"
#include "MCM/MCMTable.h"

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
            } break;
            case SKSE::MessagingInterface::kInputLoaded: {
                RE::BSInputDeviceManager::GetSingleton()->AddEventSink(Events::EventListener::GetSingleton());
            } break;
            case SKSE::MessagingInterface::kDataLoaded: {
                // needs to be in here because a lot of these need to access forms
                GameAPI::GameTable::setup();

                Sound::SoundTable::setup();
                Graph::GraphTable::SetupActions();
                Trait::TraitTable::setup();
                Alignment::Alignments::LoadAlignments();
                Papyrus::Build();

                Compatibility::CompatibilityTable::setupForms();
                Util::LookupTable::setupForms();
                Trait::TraitTable::setupForms();
                MCM::MCMTable::setupForms();
                Furniture::FurnitureTable::setupForms();
                Graph::GraphTable::setupEvents();

                UI::Align::AlignMenu::Register();
                
                // we are installing this hook so late because we need it to overwrite the PapyrusUtil hook
                Events::PackageStart::Install();
            } break;
            case SKSE::MessagingInterface::kNewGame: {
                UI::Align::AlignMenu::Show();
            }break;
            case SKSE::MessagingInterface::kPostPostLoad: {
                UI::Align::AlignMenu::Show();
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

    InterfaceMap::GetSingleton()->AddInterface("Messaging", Messaging::MessagingRegistry::GetSingleton());

    auto message = SKSE::GetMessagingInterface();
    if (!message->RegisterListener(MessageHandler)) {
        return false;
    }

    Patch::Install();
    Papyrus::Bind();

    const auto serial = SKSE::GetSerializationInterface();
    serial->SetUniqueID(_byteswap_ulong('OST'));
    serial->SetSaveCallback(Serialization::Save);
    serial->SetLoadCallback(Serialization::Load);
    serial->SetRevertCallback(Serialization::Revert);

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
