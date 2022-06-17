#include "MostWanted.h"
#include "injector/injector.hpp"
#include "Globals.h"

namespace Game::MostWantedApi
{
	int FEDatabase = 0x0091CF90;
	char* HudTable = (char*)0x0092FD94;
	auto FEngHud_IsHudVisible = (bool(__thiscall*)(void*))0x005A0C20;
	auto FEngHud_DetermineHudFeatures = (__int64(__thiscall*)(void* _this, signed int var1))0x0057CA60;
	auto IsDragRace = (bool(__stdcall*)())0x004D6150;

	bool IsHudEnabled = false;
	bool IsNosInstalled = false;
	bool IsBoostInstalled = false;

	unsigned __int64 __fastcall AfterDetermineHudFeatures(void* _this, int param, unsigned __int64 result)
	{
		IsHudEnabled = false;

		if (!IsDragRace())
		{
			IsHudEnabled = GetBit64(result, 1) && FEngHud_IsHudVisible(_this);
			IsNosInstalled = GetBit64(result, 11);
			IsBoostInstalled = GetBit64(result, 17);

			ClearBit64(result, 1);
			ClearBit64(result, 11); // NOS
			ClearBit64(result, 17); // Boost
			ClearBit64(result, 18);
			ClearBit64(result, 27);
		}

		return result;
	}

	__int64 __fastcall DetermineHudFeatures(void* _this, int param, int v2)
	{
		unsigned __int64 result = FEngHud_DetermineHudFeatures(_this, v2);

		return AfterDetermineHudFeatures(_this, param, result);
	}

	void __declspec(naked) HideEverySingleHudCave()
	{
		static constexpr auto Exit = 0x0057D0F5;

		__asm
		{
			mov eax, 0x0092FD94;
			mov eax, [eax];
			mov IsHudEnabled, 0;
			jmp Exit;
		}
	}
}

namespace Game
{
	MostWanted::MostWanted()
	{
		injector::MakeCALL(0x0058CA50, MostWantedApi::DetermineHudFeatures, true);
		this->HookHideEverySingleHud();
	}

	void MostWanted::HookHideEverySingleHud()
	{
		injector::MakeJMP(0x0057D0F0, MostWantedApi::HideEverySingleHudCave, true);
	}

	int MostWanted::Device()
	{
		return 0x00982BDC;
	}

	float MostWanted::GetBoost()
	{
		unsigned int offsets[] = { 0x0092FD98, 0x2dc, 0x3c };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	bool MostWanted::IsBoostInstalled()
	{
		return MostWantedApi::IsBoostInstalled;
	}

	float MostWanted::GetNos()
	{
		unsigned int offsets[] = { 0x0092FD98, 0x2e4, 0x38 };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	bool MostWanted::IsNosInstalled()
	{
		return MostWantedApi::IsNosInstalled;
	}

	float MostWanted::GetSpeedBreaker()
	{
		unsigned int offsets[] = { 0x0092FD98, 0x2e8, 0x38 };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	float MostWanted::GetSpeed()
	{
		unsigned int offsets[] = { 0x0092FD98, 0x2c0, 0x48 };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return LocalizeSpeed(*ptr);
		}

		return 0;
	}

	bool MostWanted::IsHudVisible()
	{
		return MostWantedApi::IsHudEnabled && *MostWantedApi::HudTable;
	}

	float MostWanted::GetRPM()
	{
		unsigned int offsets[] = { 0x0092FD98, 0x2c4, 0x54 };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	float MostWanted::GetRedline()
	{
		unsigned int offsets[] = { 0x0092FD98, 0x2c4, 0x58 };
		auto ptr = (float*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	int MostWanted::GetGear()
	{
		unsigned int offsets[] = { 0x0092FD98, 0x2c4, 0x60 };
		auto ptr = (int*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	bool MostWanted::GetUnits()
	{
		unsigned int offsets[] = { MostWantedApi::FEDatabase, 0x10, 0x3b };
		auto ptr = (bool*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return false;
	}

	bool MostWanted::IsInPerfectLaunchRange()
	{
		unsigned int offsets[] = { 0x0092FD98, 0x2c4, 0x65 };
		auto ptr = (bool*)GetPtr(offsets, 3);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	std::string MostWanted::GetCarName()
	{
		if (this->IsHudVisible())
		{
			unsigned int offsets[] = { 0x0092C510, 0x10, 0x1C, 0 };
			char* str = (char*)GetPtr(offsets, 4);
			if (str)
			{
				return str;
			}
		}

		return std::string();
	}
}