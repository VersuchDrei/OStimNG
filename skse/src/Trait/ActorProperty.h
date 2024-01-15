#pragma once

#include "GameAPI/GameCondition.h"

namespace Trait {
	template<class T>
	struct ActorProperty {
        GameAPI::GameCondition condition;
        T value;
	};
}