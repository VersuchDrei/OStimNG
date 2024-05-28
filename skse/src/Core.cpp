#include "Core.h"

#include "ActorProperties/ActorPropertyTable.h"
#include "Alignment/Alignments.h"
#include "Furniture/FurnitureTable.h"
#include "Graph/GraphTable.h"
#include "MCM/MCMTable.h"
#include "PluginInterfaceImplementation/InterfaceMapImpl.h"
#include "Util/Integrity.h"
#include "Sound/SoundTable.h"
#include "Trait/TraitTable.h"
#include "Util/APITable.h"
#include "Util/CompatibilityTable.h"
#include "Util/Globals.h"
#include "Util/LegacyUtil.h"
#include "Util/LookupTable.h"

namespace Core {
    void init() { 
        RNGUtil::setup();

        Interface::InterfaceMapImpl::getSingleton()->setupInterfaces();
    }

    void postLoad() {
        Util::Globals::setSceneIntegrityVerified(Integrity::verifySceneIntegrity());
        Util::Globals::setTranslationIntegrityVerified(Integrity::verifyTranslationIntegrity());
    }

    void postpostLoad() {
        
    }

    void dataLoaded() {
        ActorProperties::ActorPropertyTable::setup();
        Util::APITable::setupForms();
        Util::Globals::setupForms();
        Sound::SoundTable::setup();
        Graph::GraphTable::SetupActions();
        Trait::TraitTable::setup();
        Alignment::Alignments::LoadAlignments();
        Furniture::FurnitureTable::setupFurnitureTypes();
        LegacyUtil::loadLegacyScenes();
        Graph::GraphTable::setupNodes();
        Graph::GraphTable::setupSequences();

        Compatibility::CompatibilityTable::setupForms();
        Util::LookupTable::setupForms();
        Trait::TraitTable::setupForms();
        MCM::MCMTable::setupForms();
        Graph::GraphTable::setupEvents();
        Graph::GraphTable::setupOptions();
    }

    void postDataLoaded() { }

    void save(GameAPI::GameSerializationInterface serial) { }

    void load(GameAPI::GameSerializationInterface serial) { }

    void revert(GameAPI::GameSerializationInterface serial) { }
}  // namespace Core