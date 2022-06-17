#pragma once
#include "BaseGame.h"

namespace Game
{
	class Underground2 : public BaseGame
	{
	public:
		Underground2();

		int Device();
		float GetShake();
		float GetBoost();
		bool IsBoostInstalled();
		float GetNos();
		bool IsNosInstalled();
		float GetSpeedBreaker();
		float GetSpeed();
		bool IsHudVisible();
		float GetRPM();
		float GetRedline();
		int GetGear();
		bool GetUnits();
		bool IsInPerfectLaunchRange();
		bool IsSpeedBreakerInstalled();
		std::string GetCarName();
	};
}