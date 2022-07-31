#include "Underground.h"

#include "injector/injector.hpp"

namespace Game::UndergroundApi
{
	auto Game_DetermineHudFeatures = (unsigned __int64(__stdcall*)(void* _this))0x000042E620;
	auto Game_GetBoost = (double(__thiscall*)(bool*))0x0042A6B0;

	bool IsHudEnabled = false;

	bool IsDragMode()
	{
		return *(bool*)0x0078A344 || *(bool*)0x007361A8;
	}

	// Wrap usercall to get boost function
	float __declspec(naked) __fastcall GetBoost(void*, void*)
	{
		__asm
		{
			mov eax, edx;
			call Game_GetBoost;
			ret;
		}
	}

	unsigned __int64 __stdcall DetermineHudFeatures(void* _this)
	{
		IsHudEnabled = false;

		auto result = Game_DetermineHudFeatures(_this);
		if (!IsDragMode())
		{
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
	Underground::Underground()
	{
		injector::MakeCALL(0x00430DBD, UndergroundApi::DetermineHudFeatures, true);
	}

	int Underground::Device()
	{
		return 0x0073636C;
	}

	float Underground::GetShake()
	{
		return 0.0f;
	}

	float Underground::GetBoost()
	{
		unsigned int offsets[] = { 0x007361BC, 4, 0 };
		float* ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			bool result;
			return UndergroundApi::GetBoost(&result, ptr) * 20.0f;
		}

		return 0;
	}

	bool Underground::IsBoostInstalled()
	{
		unsigned int offsets[] = { 0x007361BC, 4, 0 };
		auto ptr = GetPtr(offsets, 3);
		if (ptr)
		{
			bool result;
			UndergroundApi::GetBoost(&result, ptr);
			return result;
		}

		return false;
	}

	float Underground::GetNos()
	{
		unsigned int offsets[] = { 0x007361BC, 4, 0x3f0 };
		auto maxNosPtr = (int*)GetPtr(offsets, 3);
		if (maxNosPtr)
		{
			auto nosPtr = maxNosPtr + 2;
			auto nosDelta = maxNosPtr + 3;
			int* worldTimer = (int*)0x0073AD34;
			if (*nosDelta)
			{
				return (float)(*nosPtr - (*worldTimer - *nosDelta)) / *maxNosPtr;
			}

			return (float)*nosPtr / *maxNosPtr;
		}

		return 0;
	}

	bool Underground::IsNosInstalled()
	{
		unsigned int offsets[] = { 0x007361BC, 4, 0x3f0 };
		auto ptr = (int*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr != 0;
		}

		return 0;
	}

	float Underground::GetSpeedBreaker()
	{
		return 0.0f;
	}

	float Underground::GetSpeed()
	{
		unsigned int offsets[] = { 0x007361BC, 4, 0x3d4 };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return LocalizeSpeed(*ptr);
		}

		return 0;
	}

	bool Underground::IsHudVisible()
	{
		return UndergroundApi::IsHudEnabled;
	}

	float Underground::GetRPM()
	{
		unsigned int offsets[] = { 0x007361BC, 4, 0x3e0 };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	float Underground::GetRedline()
	{
		float* redlinePtr = (float*)0x0077C9F4;
		return *redlinePtr;
	}

	int Underground::GetGear()
	{
		unsigned int offsets[] = { 0x007361BC, 4, 0x3d8 };
		auto ptr = (int*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	bool Underground::GetUnits()
	{
		auto ptr = (bool*)0x0078A3F5;
		return *ptr;
	}

	bool Underground::IsInPerfectLaunchRange()
	{
		return false;
	}

	bool Underground::IsSpeedBreakerInstalled()
	{
		return false;
	}

	std::string Underground::GetCarName()
	{
		unsigned int offsets[] = { 0x007361BC, 4, 0xC, 0 };
		auto ptr = (char*)GetPtr(offsets, 4);
		if (ptr)
		{
			return ptr;
		}

		return std::string();
	}
}
