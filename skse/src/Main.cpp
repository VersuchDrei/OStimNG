#include <stddef.h>

#include "Alignment/Alignments.h"
#include "Events/EventListener.h"
#include "Furniture/FurnitureTable.h"
#include "Game/Patch.h"
#include "Graph/LookupTable.h"
#include "InterfaceSpec/IPluginInterface.h"
#include "InterfaceSpec/PluginInterface.h"
#include "Messaging/IMessages.h"
#include "Papyrus/Papyrus.h"
#include "SKEE.h"
#include "Serial/Manager.h"
#include "Trait/TraitTable.h"
#include "UI/Align/AlignMenu.h"
#include "Util/MCMTable.h"

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

                auto message = SKSE::GetMessagingInterface();
                if (message) {
                    message->RegisterListener(nullptr, UnspecificedSenderMessageHandler);
                }
            } break;
            case SKSE::MessagingInterface::kDataLoaded: {
                Graph::LookupTable::setupForms();
                Trait::TraitTable::setupForms();
                MCM::MCMTable::setupForms();
                Furniture::FurnitureTable::setupForms();

                UI::Align::AlignMenu::Register();
                
                // we are installing this hook so late because we need it to overwrite the PapyrusUtil hook
                Events::PackageStart::Install();
            } break;
            case SKSE::MessagingInterface::kNewGame: {
                UI::Align::AlignMenu::Show();
            }break;
            case SKSE::MessagingInterface::kPostPostLoad: {
                UI::Align::AlignMenu::Show();
                SKEE::InterfaceExchangeMessage msg;
                auto intfc = SKSE::GetMessagingInterface();
                intfc->Dispatch(SKEE::InterfaceExchangeMessage::kExchangeInterface, (void*)&msg, sizeof(SKEE::InterfaceExchangeMessage*), "skee");
                if (!msg.interfaceMap) {
                    logger::critical("Couldn't get interface map!");
                    return;
                }

                auto nioInterface = static_cast<SKEE::INiTransformInterface*>(msg.interfaceMap->QueryInterface("NiTransform"));
                if (nioInterface) {
                    logger::info("NiTransform version {}", nioInterface->GetVersion());
                    Graph::LookupTable::setNiTransfromInterface(nioInterface);
                } else {
                    logger::critical("Couldn't get NiTransformInterface!");
                }
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
    Graph::LookupTable::SetupActions();
    Trait::TraitTable::setup();
    Alignment::Alignments::LoadAlignments();
    Papyrus::Build();

    const auto serial = SKSE::GetSerializationInterface();
    serial->SetUniqueID(_byteswap_ulong('OST'));
    serial->SetSaveCallback(Serialization::Save);
    serial->SetLoadCallback(Serialization::Load);
    serial->SetRevertCallback(Serialization::Revert);

    log::info("{} has finished loading.", plugin->GetName());
    return true;
}
