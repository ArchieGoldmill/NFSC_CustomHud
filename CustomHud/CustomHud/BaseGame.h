#pragma once
#include "GameApi.h"

namespace Game
{
	class BaseGame : public Api
	{
	public:
		float GetShake();
		void Detour();
		bool IsSpeedBreakerInstalled();
		bool IsNosCountInstalled();
		int GetNosCount();
	};
}