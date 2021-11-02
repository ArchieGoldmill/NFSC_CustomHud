#pragma once

#include "BaseGame.h"

namespace Game
{
	class Undercover : public BaseGame
	{
	public:
		Undercover();

		int Device();
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
		std::string GetCarName();
	};
}