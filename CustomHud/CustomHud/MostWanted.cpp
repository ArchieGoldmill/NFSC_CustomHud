#include "MostWanted.h"
#include "injector/injector.hpp"
#include "Globals.h"

namespace MWApi
{
	void** PVehicle = (void**)0x0092CD28;
	int FEDatabase = 0x0091CF90;
	int EnginePtr = 0x913E80;
	char* HudTable = (char*)0x0092FD94;
	auto PVehicle_GetSpeed = (float(__thiscall*)(void*))0x006881B0;
	auto EngineRacer_HasNOS = (bool(__thiscall*)(void*))0x006A0430;
	auto EngineRacer_GetInductionPSI = (float(__thiscall*)(void*))0x006A0520;
	auto EngineRacer_GetGear = (int(__thiscall*)(void*))0x006A0590;
	auto EngineRacer_GetRPM = (float(__thiscall*)(void*))0x006A03A0;
	auto EngineRacer_GetNOSCapacity = (float(__thiscall*)(void*))0x006A03F0;
	auto EngineRacer_GetRedline = (float(__thiscall*)(void*))0x006A03D0;
	auto EngineRacer_InductionType = (char(__thiscall*)(void*))0x006A0510;
	auto EngineRacer_GetPerfectLaunchRange = (float(__thiscall*)(void*, float*))0x006A02F0;
	auto IsInZone = (int(__thiscall*)(void*, int))0x6702D0;
	auto FEngHud_IsHudVisible = (bool(__thiscall*)(void*))0x005A0C20;
	auto FEngHud_DetermineHudFeatures = (__int64(__thiscall*)(void* _this, signed int var1))0x0057CA60;
	auto IsDragRace = (bool(__stdcall*)())0x004D6150;

	void* GetEnginePtr(int offset = 0)
	{
		unsigned int offsets[] = { 0x913E80, 0x8, 0x60, offset };
		return (void*)GetPtr(offsets, 4);
	}

	void* GetEnginePtr2()
	{
		int offset = 0xF4;
		if (IsDragRace())
		{
			offset = 0xFC;
		}

		unsigned int offsets[] = { 0x009352B0, 0x14, offset, 0 };
		return (void*)GetPtr(offsets, 4);
	}

	bool showHud = true;

	__int64 __fastcall DetermineHudFeatures(void* _this, int v1, int v2)
	{
		__int64 result = FEngHud_DetermineHudFeatures(_this, v2);

		showHud = GetBit(result, 1) && FEngHud_IsHudVisible(_this);

		if ((!Global::HUDParams.ReplaceDragHud && IsDragRace()))
		{
			showHud = false;
			return result;
		}

		if (Global::ShowVanilla())
		{
			return result;
		}

		ClearBit(result, 1);
		ClearBit(result, 11);
		ClearBit(result, 17);
		ClearBit(result, 18);
		ClearBit(result, 27);

		return result;
	}

	void __declspec(naked) HideEverySingleHudCave()
	{
		static constexpr auto Exit = 0x0057D0F5;

		__asm
		{
			mov eax, 0x0092FD94;
			mov eax, [eax];
			mov showHud, 0;
			jmp Exit;
		}
	}
}

namespace Game
{
	MostWanted::MostWanted()
	{
		injector::MakeCALL(0x0058CA50, MWApi::DetermineHudFeatures, true);
		injector::MakeJMP(0x0057D0F0, MWApi::HideEverySingleHudCave, true);
	}

	int MostWanted::Device()
	{
		return 0x00982BDC;
	}

	float MostWanted::GetBoost()
	{
		float res = 0.0f;
		void* ptr = MWApi::GetEnginePtr2();
		if (ptr)
		{
			res = MWApi::EngineRacer_GetInductionPSI(ptr);
		}

		return res;
	}

	bool MostWanted::IsBoostInstalled()
	{
		bool res = false;

		void* ptr = MWApi::GetEnginePtr2();
		if (ptr)
		{
			res = MWApi::EngineRacer_InductionType(ptr) > 0;
		}

		return res;
	}

	float MostWanted::GetNos()
	{
		float res = 0;
		auto ptr = (char*)MWApi::GetEnginePtr();
		if (ptr)
		{
			res = MWApi::EngineRacer_GetNOSCapacity(ptr);
		}

		return res;
	}

	bool MostWanted::IsNosInstalled()
	{
		bool res = false;
		void* ptr = MWApi::GetEnginePtr();
		if (ptr)
		{
			return MWApi::EngineRacer_HasNOS(ptr);
		}

		return res;
	}

	float MostWanted::GetSpeedBreaker()
	{
		float res = 0.0f;
		unsigned int offsets[] = { 0x0092D858, 0x38 };
		float* ptr = (float*)GetPtr(offsets, 2);
		if (ptr)
		{
			res = *ptr;
		}

		return res;
	}

	float MostWanted::GetSpeed()
	{
		float speed = 0.0f;
		if (*MWApi::PVehicle)
		{
			speed = MWApi::PVehicle_GetSpeed(*MWApi::PVehicle);
			speed = LocalizeSpeed(speed);
		}

		return speed;
	}

	bool MostWanted::IsHudVisible()
	{
		return MWApi::showHud && *MWApi::HudTable;
	}

	float MostWanted::GetRPM()
	{
		float res = 0;
		void* ptr = MWApi::GetEnginePtr();
		if (ptr)
		{
			res = MWApi::EngineRacer_GetRPM(ptr);
		}

		return res;
	}

	float MostWanted::GetRedline()
	{
		float res = 0;
		void* ptr = MWApi::GetEnginePtr();
		if (ptr)
		{
			res = MWApi::EngineRacer_GetRedline(ptr);
		}

		return res;
	}

	int MostWanted::GetGear()
	{
		int gear = 0;

		auto ptr = (char*)MWApi::GetEnginePtr(-8);
		if (ptr)
		{
			gear = MWApi::EngineRacer_GetGear(ptr);
		}

		return gear;
	}

	bool MostWanted::GetUnits()
	{
		bool res = false;

		unsigned int offsets[] = { MWApi::FEDatabase, 0x10, 0x3b };
		char* ptr = (char*)GetPtr(offsets, 3);
		if (ptr)
		{
			res = *ptr == 1;
		}

		return res;
	}

	bool MostWanted::IsInPerfectLaunchRange()
	{
		bool res = false;
		unsigned int offsets[] = { 0x0090DBA4, 0xB1 };
		char* isRaceStarting = (char*)GetPtr(offsets, 2);

		if (isRaceStarting && *isRaceStarting == 1)
		{
			auto ptr = (char*)MWApi::GetEnginePtr(0x1C);
			if (ptr)
			{
				float range;
				float min = MWApi::EngineRacer_GetPerfectLaunchRange(ptr, &range);
				float max = min + range;
				float rpm = this->GetRPM();
				res = rpm >= min && rpm <= max;
			}
		}

		return res;
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

		return "";
	}
}