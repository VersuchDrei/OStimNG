#pragma once

#include "Peak/PeakType.h"
#include "ActionActor.h"
#include "ActionTag.h"
#include "../RoleMap.h"

#include "PluginInterface/Graph/ActionType.h"
#include "Sound/ActionSound/SoundType.h"

namespace Graph {
    namespace Action {
        struct ActionAttributes : public OStim::ActionType {
        public:
            std::string type;
            RoleMap<ActionActor> roles;
            Peak::PeakType* peakType = nullptr;
            std::vector<Sound::SoundType*> sounds;
            std::vector<ActionTag> tags;

            bool hasTag(std::string tag);
            bool hasAnyTag(std::vector<std::string> tags);
            bool hasAllTags(std::vector<std::string> tags);
            bool hasOnlyTags(std::vector<std::string> tags);

            float getStimulation(Role role, GameAPI::GameActor actor);
            float getMaxStimulation(Role role, GameAPI::GameActor actor);


            virtual const char* getActionID() override;
            
            virtual bool hasTag(const char* tag) override;
            virtual uint32_t getTagCount() override;
            virtual OStim::ActionTag* getTag(uint32_t index) override;
            virtual void forEachTag(OStim::ActionTagVisitor* visitor) override;
        };
    }
}