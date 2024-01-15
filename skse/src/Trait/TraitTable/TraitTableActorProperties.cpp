#include "Trait/TraitTable.h"

#include "Util/JsonFileLoader.h"
#include "Util/JsonUtil.h"
#include "Util/StringUtil.h"

namespace Trait {
    const char* ACTOR_PROPERTY_FILE_PATH{"Data/SKSE/Plugins/OStim/actor properties"};

    void TraitTable::setupActorProperties(){
        Util::JsonFileLoader::LoadFilesInFolder(ACTOR_PROPERTY_FILE_PATH, [&](std::string path, std::string filename, json json) {
            GameAPI::GameCondition condition;
            JsonUtil::loadGameRecord(json, condition, "condition", filename, "actor property", path, true);
            if (!condition) {
                return;
            }

            std::string type;
            JsonUtil::loadString(json, type, "type", filename, "actor property", false);
            if (!type.empty()) {
                actorTypes.push_back({condition, type});
            }
        });

        std::stable_sort(actorTypes.begin(), actorTypes.end(), [&](ActorProperty<std::string> a, ActorProperty<std::string> b) {
            return a.condition.getPriority() > b.condition.getPriority();
        });
    }


    std::string TraitTable::getActorType(GameAPI::GameActor actor) {
        for (ActorProperty<std::string> type : actorTypes) {
            if (type.condition.fulfills(actor)) {
                return type.value;
            }
        }

        return "";
    }
}