#include "Underground2.h"

#include "injector/injector.hpp"

namespace Game::Underground2Api
{
	auto Game_DetermineHudFeatures = (unsigned __int64(__thiscall*)(void* _this))0x00600090;
	auto Game_GetBoost = (float(__thiscall*)(void*, bool*))0x005ECEC0;
	auto Game_GetNosPercentageLeft = (double(__thiscall*)(void*))0x005925B0;

	bool IsHudEnabled = false;
	bool IsNosInstalled = false;

	bool IsDragMode()
	{
		return *(bool*)0x0089E7D8 || *(bool*)0x00890108;
	}

	unsigned __int64 __fastcall DetermineHudFeatures(void* _this)
	{
		IsHudEnabled = false;

		auto result = Game_DetermineHudFeatures(_this);
		if (!IsDragMode())
		{
			IsNosInstalled = GetBit64(result, 50);
			IsHudEnabled = GetBit64(result, 6);

			ClearBit64(result, 6); // Tachometer
			ClearBit64(result, 31); // Drift angle
			ClearBit64(result, 50); // Nos
		}

		return result;
	}
}

namespace Game
{
	Underground2::Underground2()
	{
		injector::MakeCALL(0x00600595, Underground2Api::DetermineHudFeatures, true);
	}

	int Underground2::Device()
	{
		return 0x00870974;
	}

	float Underground2::GetShake()
	{
		return 0.0f;
	}

	float Underground2::GetBoost()
	{
		unsigned int offsets[] = { 0x008900AC, 4, 0 };
		auto ptr = GetPtr(offsets, 3);
		if (ptr)
		{
			bool result;
			return Underground2Api::Game_GetBoost(ptr, &result) * 20.0f;
		}

		return 0;
	}

	bool Underground2::IsBoostInstalled()
	{
		unsigned int offsets[] = { 0x008900AC, 4, 0 };
		auto ptr = GetPtr(offsets, 3);
		if (ptr)
		{
			bool result;
			Underground2Api::Game_GetBoost(ptr, &result);
			return result;
		}

		return false;
	}

	float Underground2::GetNos()
	{
		unsigned int offsets[] = { 0x008900AC, 4, 0x40 };
		auto ptr = GetPtr(offsets, 3);
		if (ptr)
		{
			float nos = Underground2Api::Game_GetNosPercentageLeft(ptr);
			nos /= 2;

			return nos;
		}

		return 0.0f;
	}

	bool Underground2::IsNosInstalled()
	{
		return Underground2Api::IsNosInstalled;
	}

	float Underground2::GetSpeedBreaker()
	{
		return 0.0f;
	}

	float Underground2::GetSpeed()
	{
		unsigned int offsets[] = { 0x008900AC, 4, 0x430 };
		float* ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return LocalizeSpeed(*ptr);
		}

		return 0;
	}

	bool Underground2::IsHudVisible()
	{
		return Underground2Api::IsHudEnabled;
	}

	float Underground2::GetRPM()
	{
		unsigned int offsets[] = { 0x008900AC, 4, 0x968, 0x58 };
		float* ptr = (float*)GetPtr(offsets, 4);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	float Underground2::GetRedline()
	{
		float* redlinePtr = (float*)0x008373E4;
		return *redlinePtr;
	}

	int Underground2::GetGear()
	{
		unsigned int offsets[] = { 0x008900AC, 4, 0x434 };
		int* ptr = (int*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	bool Underground2::GetUnits()
	{
		auto ptr = (bool*)0x0083AA1A;
		return *ptr;
	}

	bool Underground2::IsInPerfectLaunchRange()
	{
		return false;
	}

	bool Underground2::IsSpeedBreakerInstalled()
	{
		return false;
	}

	std::string Underground2::GetCarName()
	{
		unsigned int offsets[] = { 0x008900AC, 4, 0xC, 0 };
		auto ptr = (char*)GetPtr(offsets, 4);
		if (ptr)
		{
			return ptr;
		}

		return std::string();
	}
}