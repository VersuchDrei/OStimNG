#include <Core/Singleton.h>
#include <InterfaceSpec/IPluginInterface.h>

namespace Interfaces {
	class ThreadInterface final :
		public IThreadInterface,
		public OStim::ISingleton<ThreadInterface> {
	public:
		virtual UINT32 GetVersion() override { return 0; }
		virtual void Revert() override {};
	public:
		OStim::Thread* GetThread(int64_t threadId);
	};
}