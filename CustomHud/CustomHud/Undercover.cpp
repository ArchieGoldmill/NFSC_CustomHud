#include "Undercover.h"
#include "injector/injector.hpp"

namespace UndercoverApi
{
	bool ShowHud = false;
	bool* IsPaused = (bool*)0x00D8E238;
	int* GameState = (int*)0x00DA57B8;

	auto FEngHud_DetermineHudFeatures = (__int64(__thiscall*)(void* _this, signed int var1))0x005CA090;

	__int64 __fastcall DetermineHudFeatures(void* _this, int v1, int v2)
	{
		int result = FEngHud_DetermineHudFeatures(_this, v2);

		ShowHud = GetBit(result, 0);
		ClearBit(result, 0);
		ClearBit(result, 2);
		ClearBit(result, 3);

		return result;
	}
}

namespace Game
{
	unsigned int UC_PVehicle = 0x00DE93E0;

	Undercover::Undercover()
	{
		injector::MakeCALL(0x005D5113, UndercoverApi::DetermineHudFeatures, true);
	}

	int Undercover::Device()
	{
		return 0x00EA0110;
	}
	float Undercover::GetBoost()
	{
		//unsigned int offsets[] = { UC_PVehicle, 0x3C, 0x1E0 };
		//auto ptr = (float*)GetPtr(offsets, 3);
		//if (ptr)
		//{
		//	return *ptr;
		//}

		return 0;
	}
	bool Undercover::IsBoostInstalled()
	{
		return true;
	}
	float Undercover::GetNos()
	{
		unsigned int offsets[] = { UC_PVehicle, 0x3C, 0xB4 };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}
	bool Undercover::IsNosInstalled()
	{
		return true;
	}
	float Undercover::GetSpeedBreaker()
	{
		unsigned int offsets[] = { 0x00DE99B8, 0x34 };
		auto ptr = (float*)GetPtr(offsets, 2);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}
	float Undercover::GetSpeed()
	{
		unsigned int offsets[] = { UC_PVehicle, 0x68 };
		auto ptr = (float*)GetPtr(offsets, 2);
		if (ptr)
		{
			return LocalizeSpeed(*ptr);
		}

		return 0;
	}
	bool Undercover::IsHudVisible()
	{
		return UndercoverApi::ShowHud && 
			!*UndercoverApi::IsPaused && 
			*UndercoverApi::GameState == 6;
	}
	float Undercover::GetRPM()
	{
		unsigned int offsets[] = { UC_PVehicle, 0x3C, 0xEC };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}
	float Undercover::GetRedline()
	{
		unsigned int offsets[] = { UC_PVehicle, 0x3C, 0xE0, 0xADC };
		auto ptr = (float*)GetPtr(offsets, 4);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}
	int Undercover::GetGear()
	{
		unsigned int offsets[] = { UC_PVehicle, 0x3C, 0x38 };
		auto ptr = (int*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}
	bool Undercover::GetUnits()
	{
		unsigned int offsets[] = { 0x01333EA4, 0x20, 0x14 };
		auto ptr = (int*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}
	bool Undercover::IsInPerfectLaunchRange()
	{
		return false;
	}
	std::string Undercover::GetCarName()
	{
		return std::string();
	}
}