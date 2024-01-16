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

            JsonUtil::consumeLowerString(json, [condition](std::string type) {actorTypes.add({condition, type});}, "type", filename, "actor property", false);
            JsonUtil::consumeLowerString(json, [condition](std::string expression) {actorExpressions.add({condition, expression});}, "expression", filename, "actor property", false);
            JsonUtil::consumeBool(json, [condition](bool mute) {actorMutes.add({condition, mute});}, "mute", filename, "actor property", false);
            JsonUtil::consumeBool(json, [condition](bool muffle) {actorMuffles.add({condition, muffle});}, "muffle", filename, "actor property", false);
            JsonUtil::consumeBoolMap(json, [condition](std::string requirement, bool meet){actorRequirements.insert({requirement, {}}).first->second.add({condition, meet});}, true, "requirements", filename, "actor property", false);
        });

        actorTypes.sort();
        actorExpressions.sort();
        actorMutes.sort();
        actorMuffles.sort();

        for (auto& [type, list] : actorRequirements) {
            list.sort();
        }
    }

    std::set<std::string> TraitTable::getActorRequirements(GameAPI::GameActor actor) {
        std::set<std::string> requirements;

        if (actor) {
            for (auto& [id, list] : actorRequirements) {
                if (list.get(actor, false)) {
                    requirements.insert(id);
                }
            }
        } else {
            for (auto& [id, list] : actorRequirements) {
                requirements.insert(id);
            }
        }

        return requirements;
    }
}