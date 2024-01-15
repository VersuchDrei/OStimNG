#pragma once

#include "ThreadStarter/ThreadStartParams.h"

#include <shared_mutex>

#include "AutoModeStage.h"
#include "ThreadActor.h"

#include "Alignment/ActorAlignment.h"
#include "Alignment/ThreadKey.h"
#include "GameAPI/GameActor.h"
#include "Graph/Node.h"
#include "Serial/OldThread.h"
#include "Util/VectorUtil.h"

namespace OStim {

    using ThreadId = int64_t;
    class Thread : public RE::BSTEventSink<RE::BSAnimationGraphEvent>{
    public:
        ThreadId m_threadId;

        Thread(int threadID, ThreadStartParams params);

        ~Thread();

        void initContinue();

        inline ThreadFlags getThreadFlags() { return threadFlags; }

        inline Graph::Node* getCurrentNode() { return m_currentNode; }
        inline int getCurrentSpeed() { return m_currentNodeSpeed; }

        std::string getAlignmentKey();
        Alignment::ActorAlignment getActorAlignment(int index);
        void updateActorAlignment(int index, Alignment::ActorAlignment alignment);
        void alignActors();

        inline void setStopTimer(int timer) { stopTimer = timer; }
        void Navigate(std::string sceneId);

        void ChangeNode(Graph::Node* a_node);

        int getActorCount() { return m_actors.size(); }

        void AddActor(RE::Actor* a_actor);
        void RemoveActor();

        std::vector<Trait::ActorCondition> getActorConditions();

        void loop();

        inline std::map<int32_t, ThreadActor>& getActors() { return m_actors; }

        ThreadActor* GetActor(GameAPI::GameActor a_actor);
        ThreadActor* GetActor(int a_position);
        int getActorPosition(GameAPI::GameActor actor);
        std::vector<GameAPI::GameActor> getGameActors();

        void SetSpeed(int speed);
        bool increaseSpeed();
        bool decreaseSpeed();
        inline int getSpeed() { return m_currentNodeSpeed; }
        inline float getRelativeSpeed() { return m_currentNode ? static_cast<float>(m_currentNodeSpeed) / static_cast<float>(m_currentNode->speeds.size()) : 0; }

        float getMaxExcitement();

        void callEvent(std::string eventName, int actorIndex, int targetIndex, int performerIndex);

        void stop();
        void stopFaded();
        void close();

        inline bool isPlayerThread() { return playerThread; }
    public:
        virtual RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override;

        Serialization::OldThread serialize();

    private:
        ThreadFlags threadFlags = 0;

        Graph::Node* m_currentNode = nullptr;
        bool playerThread = false;
        std::map<int32_t, ThreadActor> m_actors;

        std::shared_mutex nodeLock;

        GameAPI::GamePosition center;

        int m_currentNodeSpeed = 0;
        float relativeSpeed = -1;

        std::string alignmentKey;

        float freeCamSpeedBefore = 0;
        float worldFOVbefore = 0;
        float timeScaleBefore = 0;

        int stopTimer = 0;
        bool isStopping = false;

        std::vector<Sound::SoundPlayer*> soundPlayers;

        void addActorInner(int index, RE::Actor* actor);
        void addActorSink(RE::Actor* a_actor);
        void removeActorSink(RE::Actor* a_actor);

        void rebuildAlignmentKey();
        void alignActor(ThreadActor* threadActor, Alignment::ActorAlignment alignment);

#pragma region autocontrol
    public:
        inline bool isInAutoMode() { return autoMode; }
        void startAutoMode();
        inline void stopAutoMode() { autoMode = false; }

        void setAutoModeToMainStage();

    private:
        int autoSpeedControlCooldown = 5000;

        bool autoMode = false;
        AutoModeStage autoModeStage = AutoModeStage::NONE;
        int foreplayThreshold = 0;
        int pulloutThreshold = 0;
        int autoModeCooldown = 0;

        void evaluateAutoMode();
        void startAutoModeCooldown();
        void progressAutoMode();
        void loopAutoControl();
        void nodeChangedAutoControl();
#pragma endregion

#pragma region climax
    public:
        inline bool getStallClimax() { return stallClimax; }
        inline void setStallClimax(bool stallClimax) { this->stallClimax = stallClimax; }
        bool isAnyActorAwaitingClimax();

    private:
        bool stallClimax = false;
#pragma endregion

#pragma region furniture
    public:
        inline GameAPI::GameObject getFurniture() { return furniture; }
        inline Furniture::FurnitureType* getFurnitureType() { return furnitureType; }

    private:
        GameAPI::GameObject furniture;
        Furniture::FurnitureType* furnitureType = nullptr;
        GameAPI::GameOwnership furnitureOwner = nullptr;
        float furnitureScaleMult = 1.0f;
#pragma endregion

#pragma region metadata
    public:
        inline void addMetadata(std::string metadata) {
            if (!hasMetadata(metadata)) this->metadata.push_back(metadata);
        }
        inline bool hasMetadata(std::string metadata) { return VectorUtil::contains(this->metadata, metadata); }
        inline std::vector<std::string> getMetadata() { return metadata; }

    private:
        std::vector<std::string> metadata;
#pragma endregion

#pragma region navigation
    public:
        void loopNavigation();
        bool autoTransition(std::string type);
        bool autoTransition(int index, std::string type);
        void warpTo(Graph::Node* node, bool useFades);
        void navigateTo(Graph::Node* node);
        void playSequence(Graph::Sequence* sequence, bool navigateTo, bool useFades);
        void playSequence(std::vector<Graph::SequenceEntry> nodes, bool navigateTo, bool useFades);
        bool pullOut();
        inline bool areNodesQueued() { return !nodeQueue.empty(); }
        inline bool isInSequence() { return inSequence; }

    private:
        bool inSequence = false;
        bool endAfterSequence = false;
        int nodeQueueCooldown = 0;
        std::queue<Graph::SequenceEntry> nodeQueue;

        void clearNodeQueue();
#pragma endregion

#pragma region sound
    public:
        bool isAnyActorTalking();

    private:
        bool playingClimaxSound = false;
#pragma endregion
    };

}  // namespace OStim