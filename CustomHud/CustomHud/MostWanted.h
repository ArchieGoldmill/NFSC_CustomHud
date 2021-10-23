#pragma once
#include "BaseGame.h"

namespace Game::MostWantedApi
{
	__int64 __fastcall AfterDetermineHudFeatures(void* _this, int param, __int64 result);
}

namespace Game
{
	class MostWanted : public BaseGame
	{
	protected:
		void HookHideEverySingleHud();
	public:
		MostWanted();

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