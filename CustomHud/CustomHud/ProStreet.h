#pragma once

#include "BaseGame.h"

namespace Game
{
	class ProStreet : public BaseGame
	{
	public:
		ProStreet();

		int Device();
		float GetBoost();
		bool IsBoostInstalled();
		float GetNos();
		int GetNosCount();
		bool IsNosCountInstalled();
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