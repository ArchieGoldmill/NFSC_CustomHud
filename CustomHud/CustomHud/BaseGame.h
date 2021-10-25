#pragma once
#include "GameApi.h"

namespace Game
{
	class BaseGame : public Api
	{
	public:
		static bool ShowHud;

		float GetShake();
		void Detour();
		bool IsSpeedBreakerInstalled();
	};
}