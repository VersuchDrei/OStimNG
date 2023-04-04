#include "Alignments.h"

#include "ThreadAlignment.h"

namespace Alignment {
    const char* ALIGNMENT_DATA_FILE_PATH{"Data/SKSE/Plugins/OStim/alignment.json"};

    void Alignments::LoadAlignments() {
        logger::info("loading alignment data");
        fs::path rootPath{ALIGNMENT_DATA_FILE_PATH};
        if (!fs::exists(rootPath)) {
            logger::warn("file path ({}) does not exist", ALIGNMENT_DATA_FILE_PATH);
            return;
        }
        std::ifstream ifs(ALIGNMENT_DATA_FILE_PATH);
        json json = json::parse(ifs, nullptr, false);

        if (json.is_discarded()) {
            logger::warn("file {} is malformed", ALIGNMENT_DATA_FILE_PATH);
            return;
        }

        for (auto& [threadKey, threadValue] : json.items()) {
            ThreadAlignment threadAlignment;

            for (auto& [nodeKey, nodeValue] : threadValue.items()) {
                NodeAlignment nodeAlignment;

                for (auto& [index, actorValue] : nodeValue.items()) {
                    ActorAlignment actorAlignment;

                    if (actorValue.contains("offsetX")) {
                        actorAlignment.offsetX = actorValue["offsetX"];
                    }
                    if (actorValue.contains("offsetY")) {
                        actorAlignment.offsetY = actorValue["offsetY"];
                    }
                    if (actorValue.contains("offsetZ")) {
                        actorAlignment.offsetZ = actorValue["offsetZ"];
                    }
                    if (actorValue.contains("scale")) {
                        actorAlignment.scale = actorValue["scale"];
                    }
                    if (actorValue.contains("rotation")) {
                        actorAlignment.rotation = actorValue["rotation"];
                    }
                    if (actorValue.contains("sosBend")) {
                        actorAlignment.sosBend = actorValue["sosBend"];
                    }

                    nodeAlignment.actors[std::stoi(index)] = actorAlignment;
                }

                threadAlignment.nodes[nodeKey] = nodeAlignment;
            }

            threadAlignments[threadKey] = threadAlignment;
        }
        logger::info("alignment data loaded");
    }

    void Alignments::SerializeAlignments() {
        json json = json::object();

        for (auto& [threadKey, threadAlignment] : threadAlignments) {
            json[threadKey] = json::object();

            for (auto& [nodeKey, nodeAlignment] : threadAlignment.nodes) {
                json[threadKey][nodeKey] = json::object();

                for (auto& [index, actorAlignment] : nodeAlignment.actors) {
                    std::string strIndex = std::to_string(index);
                    json[threadKey][nodeKey][strIndex] = json::object();

                    json[threadKey][nodeKey][strIndex]["offsetX"] = actorAlignment.offsetX;
                    json[threadKey][nodeKey][strIndex]["offsetY"] = actorAlignment.offsetY;
                    json[threadKey][nodeKey][strIndex]["offsetZ"] = actorAlignment.offsetZ;
                    json[threadKey][nodeKey][strIndex]["scale"] = actorAlignment.scale;
                    json[threadKey][nodeKey][strIndex]["rotation"] = actorAlignment.rotation;
                    json[threadKey][nodeKey][strIndex]["sosBend"] = actorAlignment.sosBend;
                }
            }
        }

        std::ofstream db_file(ALIGNMENT_DATA_FILE_PATH);
        db_file << std::setw(2) << json << std::endl;
    }

    ActorAlignment Alignments::getActorAlignment(std::string threadKey, Graph::Node* node, int index) {
        if (!node) {
            return {};
        }

        auto threadIter = threadAlignments.find(threadKey);
        if (threadIter == threadAlignments.end()) {
            return getDefaultAlignment(node, index);
        }

        auto nodeIter = threadIter->second.nodes.find(node->scene_id);
        if (nodeIter == threadIter->second.nodes.end()) {
            return getDefaultAlignment(node, index);
        }

        auto actorIter = nodeIter->second.actors.find(index);
        if (actorIter == nodeIter->second.actors.end()) {
            return getDefaultAlignment(node, index);
        }

        return actorIter->second;
    }

    void Alignments::setActorAlignment(std::string threadKey, Graph::Node* node, int index, ActorAlignment alignment) {
        if (!node) {
            return;
        }

        if (!threadAlignments.contains(threadKey)) {
            threadAlignments[threadKey] = {};
        }

        if (!threadAlignments[threadKey].nodes.contains(node->scene_id)) {
            threadAlignments[threadKey].nodes[node->scene_id] = {};
        }

        threadAlignments[threadKey].nodes[node->scene_id].actors[index] = alignment;
    }

    ActorAlignment Alignments::getDefaultAlignment(Graph::Node* node, int index) {
        if (index >= node->actors.size()) {
            return {};
        }

        return {.sosBend = static_cast<float>(node->actors[index]->penisAngle)};
    }
}
