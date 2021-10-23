#pragma once
#include "GameApi.h"

namespace Game
{
	class BaseGame : public Api
	{
	public:
		static bool ShowHud;

		void Detour();
		bool IsSpeedBreakerInstalled();
	};
}