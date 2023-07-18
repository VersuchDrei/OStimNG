#include "Graph/GraphTable.h"

#include "Util/LegacyUtil.h"
#include "Util/StringUtil.h"

namespace Graph {
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

    void GraphTable::addNavigations(std::vector<RawNavigation> navigations) {
        for (RawNavigation& raw : navigations) {
            Node* start = getNodeById(raw.origin);
            if (!start) {
                logger::warn("Couldn't add navigation from {} to {} because {} doesn't exist.", raw.origin, raw.destination, raw.origin);
                continue;
            }

            Node* destination = getNodeById(raw.destination);
            if (!destination) {
                logger::warn("Couldn't add navigation from {} to {} because {} doesn't exist.", raw.origin, raw.destination, raw.destination);
                continue;
            }

            if (start->furnitureType != destination->furnitureType) {
                logger::warn("Couldn't add navigation from {} to {} because their furniture types don't match.", raw.origin, raw.destination);
                continue;
            }

            if (start->actors.size() != destination->actors.size()) {
                logger::warn("Couldn't add navigation from {} to {} because their actor counts don't match.", raw.origin, raw.destination);
                continue;
            }

            for (auto& existingNavigation : start->navigations) {
                if (existingNavigation.destination == destination || existingNavigation.transitionNode == destination) {
                    continue;
                }
            }

            Navigation navigation;

            if (destination->isTransition) {
                for (RawNavigation& nav : navigations) {
                    if (nav.origin == destination->scene_id) {
                        navigation.destination = GraphTable::getNodeById(nav.destination);
                        break;
                    }
                }

                if (!navigation.destination) {
                    logger::warn("Couldn't add navigation from {} to {} because {} transition destination doesn't exist.", raw.origin, raw.destination, raw.destination);
                    continue;
                }

                // TODO: what do when people chain transitions?
                if (navigation.destination->isTransition) {
                    logger::warn( "Couldn't add navigation from {} to {} because {} transition destination is another transition.", raw.origin, raw.destination, raw.destination);
                    continue;
                }

                navigation.isTransition = true;
                navigation.transitionNode = destination;
            } else {
                navigation.destination = destination;
            }

            if (std::regex_search(raw.border, Constants::hexColor)) {
                navigation.border = raw.border;
            }
            if (raw.icon == "") {
                navigation.icon = "OStim/icons/" + LegacyUtil::getIcon(navigation.destination == start ? navigation.transitionNode : navigation.destination) + ".dds";
            } else {
                navigation.icon = "OStim/icons/" + raw.icon + ".dds";
            }

            start->navigations.push_back(navigation);
        }
    }

    Node* GraphTable::getNodeById(std::string id) {
        StringUtil::toLower(&id);
        auto iter = nodes.find(id);
        if (iter != nodes.end()) {
            return iter->second;
        }
        return nullptr;
    }

    bool ContainsId(Node* n, std::string& id) {
        if (n->lowercase_id.contains(id))
            return true;
        return false;
    }

    void GraphTable::findNodesById(std::string& id, std::vector<Node*>& results) {
        StringUtil::toLower(&id);
        for (auto& iter : nodes) {
            if (ContainsId(iter.second,id)) {
                results.push_back(iter.second);
            }
        }
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

    Node* GraphTable::getRandomNode(Furniture::FurnitureType furnitureType, std::vector<Trait::ActorCondition> actorConditions, std::function<bool(Node*)> nodeCondition) {
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

}