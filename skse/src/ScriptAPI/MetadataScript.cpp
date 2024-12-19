#include "MetadataScript.h"

#include "Graph/GraphTable.h"
#include "Graph/Node.h"

namespace ScriptAPI {
    namespace Metadata {
        std::vector<std::string> getNodesInRange(std::string id, std::vector<GameAPI::GameActor> actors, int distance) {
            Graph::Node* node = Graph::GraphTable::getNodeById(id);

            if (!node) {
                return {};
            }

            std::vector<Trait::ActorCondition> actorConditions = Trait::ActorCondition::create(actors);
            std::vector<Graph::Node*> nodes = node->getNodesInRange(distance, actorConditions, [](Graph::Node* node) { return true; });

            std::sort(nodes.begin(), nodes.end(), [](Graph::Node* a, Graph::Node* b) { return a->lowercase_name < b->lowercase_name; });

            std::vector<std::string> ids;
            for (Graph::Node* node : nodes) {
                ids.push_back(node->scene_id);
            }

            return ids;
        }

        std::vector<std::string> nodesToNames(std::vector<std::string> ids) {
            std::vector<std::string> names;

            for (std::string id : ids) {
                Graph::Node* node = Graph::GraphTable::getNodeById(id);
                names.push_back(node ? node->scene_name : "");
            }

            return names;
        }
    }
}