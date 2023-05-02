#include "Graph/GraphTable.h"

#include "Graph/Node.h"
#include "Trait/Condition.h"
#include "Util/Constants.h"
#include "Util/StringUtil.h"
#include "Util/JsonFileLoader.h"
#include "SKEE.h"


namespace Graph {
    const char* ACTION_FILE_PATH{ "Data/SKSE/Plugins/OStim/actions" };

    void GraphTable::addNode(Node* node) {
        nodes.insert({node->lowercase_id, node});
        for (Graph::Speed speed : node->speeds) {
            animationNodeTable.insert({speed.animation, node});
        }

        std::unordered_map<int, std::vector<Node*>*>* innerMap;
        std::vector<Node*>* innerList;
        int count = node->actors.size();
        auto iter = nodeList.find(node->furnitureType);
        if (iter != nodeList.end()) {
            innerMap = iter->second;
        } else {
            innerMap = new std::unordered_map<int, std::vector<Node*>*>();
            nodeList.insert({node->furnitureType, innerMap});
        }

        auto iter2 = innerMap->find(count);
        if (iter2 != innerMap->end()) {
            innerList = iter2->second;
        } else {
            innerList = new std::vector<Node*>();
            innerMap->insert({count, innerList});
        }
        
        innerList->push_back(node);
    }

    Node* GraphTable::getNodeById(std::string id) {
        StringUtil::toLower(&id);
        auto iter = nodes.find(id);
        if (iter != nodes.end()) {
            return iter->second;
        }
        return nullptr;
    }

    Node* GraphTable::getNodeByAnimation(std::string anim) {
        auto iter = animationNodeTable.find(anim);
        if (iter != animationNodeTable.end()) {
            return iter->second;
        }
        return nullptr;
    }

    bool GraphTable::hasNodes(Furniture::FurnitureType furnitureType, int actorCount) {
        if (furnitureType == Furniture::FurnitureType::BED) {
            furnitureType = Furniture::FurnitureType::NONE;
        }

        auto iter = nodeList.find(furnitureType);
        if (iter == nodeList.end()) {
            return false;
        }

        auto iter2 = iter->second->find(actorCount);
        return iter2 != iter->second->end();
    }

    Node* GraphTable::getRandomNode(Furniture::FurnitureType furnitureType, std::vector<Trait::ActorConditions> actorConditions, std::function<bool(Node*)> nodeCondition) {
        if (furnitureType == Furniture::FurnitureType::BED) {
            furnitureType = Furniture::FurnitureType::NONE;
        }

        auto iter = nodeList.find(furnitureType);
        if (iter == nodeList.end()) {
            return nullptr;
        }
        auto innerMap = iter->second;
        
        auto iter2 = innerMap->find(actorConditions.size());
        if (iter2 == innerMap->end()) {
            return nullptr;
        }
        // the copy is to prevent race conditions if several scripts try to call this at once
        std::vector<Node*> copy = *iter2->second;

        std::shuffle(std::begin(copy), std::end(copy), Constants::RNG);

        for (auto& node : copy) {
            if (!node->isTransition && !node->noRandomSelection && node->fulfilledBy(actorConditions) && nodeCondition(node)) {
                return node;
            }
        }
        
        return nullptr;
    }

    void parseActor(json& json, ActionActor& actor) {
        if (json.contains("stimulation")) {
            actor.stimulation = json["stimulation"];
        }

        if (json.contains("maxStimulation")) {
            actor.maxStimulation = json["maxStimulation"];
        }

        if (json.contains("fullStrip")) {
            actor.fullStrip = json["fullStrip"];
        }

        if (json.contains("moan")) {
            actor.moan = json["moan"];
        }

        if (json.contains("talk")) {
            actor.talk = json["talk"];
        }

        if (json.contains("muffled")) {
            actor.muffled = json["muffled"];
        }

        if (json.contains("expressionOverride")) {
            actor.expressionOverride = json["expressionOverride"];
            StringUtil::toLower(&actor.expressionOverride);
        }

        if (json.contains("requirements")) {
            for (auto& req : json["requirements"]) {
                actor.requirements |= GraphTable::getRequirement(req);
            }
        }

        if (json.contains("strippingSlots")) {
            for (auto& slot : json["strippingSlots"]) {
                actor.strippingMask |= 1 << (slot.get<int>() - 30);
            }
        }

        if (json.contains("ints")) {
            for (auto& [key, val] : json["ints"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                actor.ints.insert(std::make_pair(mutableKey, val.get<int>()));
            }
        }

        if (json.contains("intLists")) {
            for (auto& [key, val] : json["intLists"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                std::vector<int> ints;
                for (auto& entry : val) {
                    ints.push_back(entry.get<int>());
                }
                actor.intLists.insert(std::make_pair(mutableKey, ints));
            }
        }

        if (json.contains("floats")) {
            for (auto& [key, val] : json["floats"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                actor.floats.insert(std::make_pair(mutableKey, val.get<float>()));
            }
        }

        if (json.contains("floatLists")) {
            for (auto& [key, val] : json["floatLists"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                std::vector<float> floats;
                for (auto& entry : val) {
                    floats.push_back(entry.get<float>());
                }
                actor.floatLists.insert(std::make_pair(mutableKey, floats));
            }
        }

        if (json.contains("strings")) {
            auto& strings = json["strings"];
            for (auto& [key, val] : json["strings"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                std::string value = val.get<std::string>();
                StringUtil::toLower(&value);
                actor.strings.insert(std::make_pair(mutableKey, value));
            }
        }

        if (json.contains("stringLists")) {
            for (auto& [key, val] : json["stringLists"].items()) {
                std::string mutableKey = key;
                StringUtil::toLower(&mutableKey);
                std::vector<std::string> strings;
                for (auto& entry : val) {
                    std::string value = entry.get<std::string>();
                    StringUtil::toLower(&value);
                    strings.push_back(value);
                }
                actor.stringLists.insert(std::make_pair(mutableKey, strings));
            }
        }
    };

    void GraphTable::SetupActions(){
        Util::JsonFileLoader::LoadFilesInFolder(ACTION_FILE_PATH, [&](std::string, std::string filename, json json) {
            Graph::ActionAttributes attr;
            if(json.contains("actor")){
                Graph::ActionActor actor;
                parseActor(json["actor"], actor);
                attr.actor = actor;
            }
            if (json.contains("target")) {
                Graph::ActionActor target;
                parseActor(json["target"], target);
                attr.target = target;
            }
            if(json.contains("performer")){
                Graph::ActionActor performer;
                parseActor(json["performer"], performer);
                attr.performer = performer;
            }
            if (json.contains("tags")) {
                for (auto& tag : json["tags"]) {
                    std::string tagStr = tag.get<std::string>();
                    StringUtil::toLower(&tagStr);
                    attr.tags.push_back(tagStr);
                }
            }
            actions.insert(std::make_pair(filename, attr));
        });
    }

    ActionAttributes* GraphTable::GetActionAttributesByType(std::string type)
    {
        if (auto it = actions.find(type); it != actions.end()) {
            return &actions.at(type);
        }
        else {
            logger::warn("No action found for {} using default", type);
            return &actions.at("default");
        }
    }

    Requirement GraphTable::getRequirement(std::string string) {
        auto iter = requirements.find(string);
        if (iter != requirements.end()) {
            return iter->second;
        }

        return Requirement::NONE;
    }
}