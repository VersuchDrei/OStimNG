#include "Papyrus\Papyrus.h"

#include "Game/LocaleManager.h"
#include "Game/Locker.h"
#include "Papyrus\Script.h"
#include "Util.h"

#define BIND(a_method, ...) a_vm->RegisterFunction(#a_method##sv, obj, a_method __VA_OPT__(, ) __VA_ARGS__)
#include "Papyrus/PapyrusActor.h"
#include "Papyrus/PapyrusAlign.h"
#include "Papyrus/PapyrusCamera.h"
#include "Papyrus/PapyrusControl.h"
#include "Papyrus/PapyrusData.h"
#include "Papyrus/PapyrusDatabase.h"
#include "Papyrus/PapyrusFace.h"
#include "Papyrus/PapyrusFurniture.h"
#include "Papyrus/PapyrusLibrary.h"
#include "Papyrus/PapyrusMetadata.h"
#include "Papyrus/PapyrusObject.h"
#include "Papyrus/PapyrusScenes.h"
#include "Papyrus/PapyrusThread.h"
#include "Papyrus/PapyrusThreadActor.h"
#include "Papyrus/PapyrusUndress.h"
#include "Papyrus/PapyrusUtil.h"
#undef BIND

namespace Papyrus {
    void Bind() {
        auto papyrus = SKSE::GetPapyrusInterface();
        if (papyrus) {
            // old stuff
            papyrus->Register(PapyrusActor::Bind);
            papyrus->Register(PapyrusCamera::Bind);
            papyrus->Register(PapyrusControl::Bind);
            papyrus->Register(PapyrusFace::Bind);
            papyrus->Register(PapyrusObject::Bind);
            papyrus->Register(PapyrusUtil::Bind);

            // new stuff
            papyrus->Register(PapyrusAlign::Bind);
            papyrus->Register(PapyrusData::Bind);
            papyrus->Register(PapyrusFurniture::Bind);
            papyrus->Register(PapyrusLibrary::Bind);
            papyrus->Register(PapyrusMetadata::Bind);
            papyrus->Register(PapyrusScenes::Bind);
            papyrus->Register(PapyrusThread::Bind);
            papyrus->Register(PapyrusThreadActor::Bind);
            papyrus->Register(PapyrusUndress::Bind);
        }
    }

    void Build() {
        PapyrusDatabase::BuildDB();
    }
}  // namespace Papyrus