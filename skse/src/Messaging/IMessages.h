#pragma once

#include <Graph/Node.h>
#include <Core/Singleton.h>
#include <InterfaceSpec/IPluginInterface.h>

namespace Messaging {

	enum messageId : uint32_t { kAnimationChanged = 0x4e9673 };
	struct MessageBase {
		uint32_t messageId;
	};
	struct AnimationChangedMessage : MessageBase {
		uint32_t messageId = kAnimationChanged;
		Graph::Node* newAnimation = nullptr;
	};

	class MessagingRegistry final:
		public IMessagingRegistry,
		public OStim::ISingleton<MessagingRegistry>
	{
	public: // IPluginInterface (from IMessagingRegistry)
		virtual UINT32 GetVersion() override { return 0; };
		virtual void Revert() override {};
	public:
		void RegisterForMessages(std::string newListener);
		template<class T>
		void SendMessageToListeners(T message){
			auto sender = SKSE::GetMessagingInterface();
			// logger::info("Sending {} to {} listeners", message.messageId, registeredListeners.size());
			for (std::string listener : registeredListeners) {
				// logger::info("Sending {} to {}", message.messageId , listener.c_str());
				sender->Dispatch(message.messageId, (void*)&message, sizeof(T), listener.c_str());
			}
		}
	private:
		std::vector<std::string> registeredListeners;
	};
}