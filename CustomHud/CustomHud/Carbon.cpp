#include <stdlib.h>
#include "Carbon.h"
#include "injector/injector.hpp"
#include <string>
#include "Globals.h"

namespace Game::CarbonApi
{
	auto DALVehicle_GetRPM = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B65F0;
	auto DALVehicle_GetMaxRPM = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B66E0;
	auto DALVehicle_GetTurbo = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6730;
	auto DALVehicle_GetSpeedMps = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B65A0;
	auto DALVehicle_GetGear = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B68C0;
	auto DALVehicle_GetNos = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B67F0;
	auto DALVehicle_GetSpeedBreaker = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6870;
	auto DALVehicle_GetCarIDHash = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6FB0;
	auto DALVehicle_GetRedLine = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6690;
	auto DALVehicle_GetWheelAngle = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004C3610;
	auto DALVehicle_GetIsPlayerControlling = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004C3540;
	auto DALVehicle_GetIsRevolutionWheelVisible = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B7050;
	auto DALVehicle_GetStockCarIDHash = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B7000;
	auto DALVehicle_GetSpeedPercentToDriftCap = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6F40;
	auto DALVehicle_GetPipHeight = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6EF0;
	auto DALVehicle_GetPipWidth = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6EA0;
	auto DALVehicle_GetPipPosY = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6E40;
	auto DALVehicle_GetPipPosX = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6DE0;
	auto DALVehicle_GetIsHudVisible = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6DA0;
	auto DALVehicle_GetIsDrivingCamera = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6D50;
	auto DALVehicle_GetIsFacingWrongWay = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6D00;
	auto DALVehicle_GetInPerfectLaunchRange = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6BF0;
	auto DALVehicle_GetPipTextureInfo = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6B90;
	auto DALVehicle_GetVehicleDestroyed = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6B40;
	auto DALVehicle_GetEngineBlown = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6AF0;
	auto DALVehicle_GetStaging = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6AA0;
	auto DALVehicle_GetShifting = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6A50;
	auto DALVehicle_GetLastShiftStatus = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B69B0;
	auto DALVehicle_GetShiftPotential = (bool(__thiscall*)(void* DALVehicle, int* getVal, const int playerNum))0x004B6910;
	auto DALVehicle_GetHeat = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6790;
	auto DALVehicle_GetSpeedLocalized = (bool(__thiscall*)(void* DALVehicle, float* getVal, const int playerNum))0x004B6500;
	auto Game_GetSpeedoUnits = (char(__stdcall*)(int* getVal))0x004A4550;

	bool IsHudEnabled = false;

	auto Game_DetermineHudFeatures = (unsigned int(__thiscall*)(void* _this, signed int var1))0x005DC4B0;
	int __fastcall DetermineHudFeatures(void* _this, int v1, int v2)
	{
		auto result = Game_DetermineHudFeatures(_this, v2);

		IsHudEnabled = GetBit(result, 1);

		ClearBit(result, 1);
		ClearBit(result, 11);
		ClearBit(result, 18);

		return result;
	}

	double Shake = 0;
	auto Game_FEngHud_UpdateShake = (double(__thiscall*)(void* _this, float a2))0x005D53A0;
	double __fastcall FEngHud_UpdateShake(void* _this, int param, float a2)
	{
		Shake = Game_FEngHud_UpdateShake(_this, a2);

		return Shake;
	}
}

namespace Game
{
	Carbon::Carbon()
	{
		injector::MakeCALL(0x005E6F7B, CarbonApi::DetermineHudFeatures, true);
		injector::MakeCALL(0x005E6FC6, CarbonApi::FEngHud_UpdateShake, true);
	}

	int Carbon::Device()
	{
		return 0x00AB0ABC;
	}

	float Carbon::GetShake()
	{
		return 1.0 - CarbonApi::Shake;
	}

	float Carbon::GetBoost()
	{
		float res = 0;
		if (this->IsHudVisible())
		{
			CarbonApi::DALVehicle_GetTurbo(NULL, &res, 0);
		}

		return res;
	}

	bool Carbon::IsBoostInstalled()
	{
		float res;
		return CarbonApi::DALVehicle_GetTurbo(NULL, &res, 0);
	}

	float Carbon::GetNos()
	{
		float nos = 0;
		if (IsHudVisible())
		{
			CarbonApi::DALVehicle_GetNos(NULL, &nos, 0);
		}

		return nos;
	}

	bool Carbon::IsNosInstalled()
	{
		if (IsHudVisible())
		{
			float nos = 0;
			return CarbonApi::DALVehicle_GetNos(NULL, &nos, 0);
		}

		return false;
	}

	float Carbon::GetSpeedBreaker()
	{
		float res = 0;
		if (IsHudVisible())
		{
			CarbonApi::DALVehicle_GetSpeedBreaker(NULL, &res, 0);
		}

		return res;
	}

	float Carbon::GetSpeed()
	{
		float speed = 0.0f;
		CarbonApi::DALVehicle_GetSpeedLocalized(NULL, &speed, 0);

		return speed;
	}

	bool Carbon::IsHudVisible()
	{
		int resHud;
		bool hud = CarbonApi::DALVehicle_GetIsHudVisible(NULL, &resHud, 0);

		int resPl;
		bool pl = CarbonApi::DALVehicle_GetIsPlayerControlling(NULL, &resPl, 0);

		return resHud != 0 && hud && CarbonApi::IsHudEnabled && resPl != 0 && pl;
	}

	float Carbon::GetRPM()
	{
		float rpm = 0.0f;
		CarbonApi::DALVehicle_GetRPM(NULL, &rpm, 0);

		return rpm;
	}

	float Carbon::GetRedline()
	{
		float rpm = 0.0f;
		CarbonApi::DALVehicle_GetRedLine(NULL, &rpm, 0);

		return rpm;
	}

	int Carbon::GetGear()
	{
		int gear = 0;
		CarbonApi::DALVehicle_GetGear(NULL, &gear, 0);

		return gear;
	}

	bool Carbon::GetUnits()
	{
		int res = 0;
		if (IsHudVisible())
		{
			CarbonApi::Game_GetSpeedoUnits(&res);
		}

		return res == 1;
	}

	bool Carbon::IsInPerfectLaunchRange()
	{
		int res;
		CarbonApi::DALVehicle_GetInPerfectLaunchRange(NULL, &res, 0);
		return res != 0;
	}

	std::string Carbon::GetCarName()
	{
		if (this->IsHudVisible())
		{
			unsigned int offsets[] = { 0x00A9F168, 0x24, 0x20, 0 };
			char* name = (char*)GetPtr(offsets, 4);
			if (name)
			{
				return name;
			}
		}

		return "";
	}
}