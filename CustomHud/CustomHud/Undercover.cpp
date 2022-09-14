#include "Undercover.h"
#include "injector/injector.hpp"

namespace UndercoverApi
{
	bool IsHudEnabled = false;
	bool* IsPaused = (bool*)0x00D8E238;
	int* GameState = (int*)0x00DA57B8;
	int* HudCount = (int*)0x01278FF8;

	auto FEngHud_DetermineHudFeatures = (unsigned __int64(__thiscall*)(void* _this, signed int var1))0x005CA090;

	__int64 __fastcall DetermineHudFeatures(int* _this, int v1, int v2)
	{
		auto result = FEngHud_DetermineHudFeatures(_this, v2);

		IsHudEnabled = GetBit(result, 0) && _this[0xA9];
		ClearBit64(result, 0);
		ClearBit64(result, 2);
		ClearBit64(result, 3);

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
		unsigned int offsets[] = { UC_PVehicle, 0x3C, 0x1C4 };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

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
		if (*UndercoverApi::IsPaused || *UndercoverApi::GameState != 6 || *UndercoverApi::HudCount == 0)
		{
			UndercoverApi::IsHudEnabled = false;
		}

		return UndercoverApi::IsHudEnabled;
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
		unsigned int offsets[] = { 0x01284610, 0x4, 0x60, 0x14 };
		auto ptr = (int*)GetPtr(offsets, 4);
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
		if (this->IsHudVisible())
		{
			unsigned int offsets[] = { UC_PVehicle, 0x50, 0x1C, 0x64, 0x30 };
			auto ptr = (char*)GetPtr(offsets, 5);
			if (ptr)
			{
				return ptr;
			}
		}

		return std::string();
	}
}