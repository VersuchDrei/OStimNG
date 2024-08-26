#include "GraphActor.h"

#include "Util/StringUtil.h"
#include "Util/VectorUtil.h"

namespace Graph {
    void GraphActor::merge(Action::ActionActor& actor) {
        for (const std::string& requirement : actor.requirements) {
            condition.requirements.insert(requirement);
        }
        moan |= actor.moan;
        talk |= actor.talk;
        muffled |= actor.muffled;
        for (GameAPI::GameFaction faction : actor.factions) {
            if (!VectorUtil::contains(factions, faction)) {
                factions.push_back(faction);
            }
        }
    }


    bool GraphActor::hasTag(std::string tag) {
        StringUtil::toLower(&tag);
        for (GraphActorTag& actorTag : tags) {
            if (actorTag.tag == tag) {
                return true;
            }
        }
        return false;
    }

    bool GraphActor::hasAnyTag(std::vector<std::string> tags) {
        for (std::string& tag : tags) {
            if (hasTag(tag)) {
                return true;
            }
        }
        return false;
    }

    bool GraphActor::hasAllTags(std::vector<std::string> tags) {
        for (std::string& tag : tags) {
            if (!hasTag(tag)) {
                return false;
            }
        }
        return true;
    }

    bool GraphActor::hasOnlyTags(std::vector<std::string> tags) {
        for (GraphActorTag& actorTag : this->tags) {
            if (!VectorUtil::contains(tags, actorTag.tag)) {
                return false;
            }
        }
        return true;
    }
    

    bool GraphActor::hasTag(const char* tag) {
        return hasTag(std::string(tag));
    }

    uint32_t GraphActor::getTagCount() {
        return tags.size();
    }

    OStim::NodeActorTag* GraphActor::getTag(uint32_t index) {
        if (index < 0 || index >= tags.size()) {
            return nullptr;
        }

        return &tags[index];
    }

    void GraphActor::forEachTag(OStim::NodeActorTagVisitor* visitor) {
        for (GraphActorTag& tag : tags) {
            if (!visitor->visit(&tag)) {
                break;
            }
        }
    }
}