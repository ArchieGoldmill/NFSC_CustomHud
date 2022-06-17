#pragma once
#include "BaseGame.h"

namespace Game
{
	class Underground : public BaseGame
	{
	public:
		Underground();

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