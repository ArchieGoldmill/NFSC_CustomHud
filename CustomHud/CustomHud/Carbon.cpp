#include <stdlib.h>
#include "Carbon.h"
#include "injector/injector.hpp"
#include <string>
#include "Globals.h"

namespace Game::CarbonApi
{
	bool(__thiscall* DALVehicle_GetRPM)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B65F0;
	bool(__thiscall* DALVehicle_GetMaxRPM)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B66E0;
	bool(__thiscall* DALVehicle_GetTurbo)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6730;
	bool(__thiscall* DALVehicle_GetSpeedMps)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B65A0;
	bool(__thiscall* DALVehicle_GetGear)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B68C0;
	bool(__thiscall* DALVehicle_GetNos)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B67F0;
	bool(__thiscall* DALVehicle_GetSpeedBreaker)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6870;
	bool(__thiscall* DALVehicle_GetCarIDHash)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6FB0;
	bool(__thiscall* DALVehicle_GetRedLine)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6690;
	bool(__thiscall* DALVehicle_GetWheelAngle)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004C3610;
	bool(__thiscall* DALVehicle_GetIsPlayerControlling)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004C3540;
	bool(__thiscall* DALVehicle_GetIsRevolutionWheelVisible)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B7050;
	bool(__thiscall* DALVehicle_GetStockCarIDHash)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B7000;
	bool(__thiscall* DALVehicle_GetSpeedPercentToDriftCap)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6F40;
	bool(__thiscall* DALVehicle_GetPipHeight)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6EF0;
	bool(__thiscall* DALVehicle_GetPipWidth)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6EA0;
	bool(__thiscall* DALVehicle_GetPipPosY)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6E40;
	bool(__thiscall* DALVehicle_GetPipPosX)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6DE0;
	bool(__thiscall* DALVehicle_GetIsHudVisible)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6DA0;
	bool(__thiscall* DALVehicle_GetIsDrivingCamera)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6D50;
	bool(__thiscall* DALVehicle_GetIsFacingWrongWay)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6D00;
	bool(__thiscall* DALVehicle_GetInPerfectLaunchRange)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6BF0;
	bool(__thiscall* DALVehicle_GetPipTextureInfo)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6B90;
	bool(__thiscall* DALVehicle_GetVehicleDestroyed)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6B40;
	bool(__thiscall* DALVehicle_GetEngineBlown)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6AF0;
	bool(__thiscall* DALVehicle_GetStaging)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6AA0;
	bool(__thiscall* DALVehicle_GetShifting)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6A50;
	bool(__thiscall* DALVehicle_GetLastShiftStatus)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B69B0;
	bool(__thiscall* DALVehicle_GetShiftPotential)(void* DALVehicle, int* getVal, const int playerNum) = (bool(__thiscall*)(void*, int*, const int))0x004B6910;
	bool(__thiscall* DALVehicle_GetHeat)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6790;
	bool(__thiscall* DALVehicle_GetSpeedLocalized)(void* DALVehicle, float* getVal, const int playerNum) = (bool(__thiscall*)(void*, float*, const int))0x004B6500;
	bool(__thiscall* DALVehicle_GetIVehicle)(const int playerNum) = (bool(__thiscall*)(const int))0x004B62B0;
	bool(__thiscall* DALVehicle_GetInt)(void* DALVehicle, const int valueType, int* getVal, const int arg1, const int arg2, const int arg3) = (bool(__thiscall*)(void*, const int, int*, const int, const int, const int))0x004CC1C0;
	bool(__thiscall* DALVehicle_GetFloat)(void* DALVehicle, const int valueType, float* getVal, const int arg1, const int arg2, const int arg3) = (bool(__thiscall*)(void*, const int, float*, const int, const int, const int))0x004CC000;
	auto Game_GetSpeedoUnits = (char(__stdcall*)(int* getVal))0x004A4550;

	auto Game_DetermineHudFeatures = (int(__thiscall*)(void* _this, signed int var1))0x005DC4B0;
	int __fastcall DetermineHudFeatures(void* _this, int v1, int v2)
	{
		int result = Game_DetermineHudFeatures(_this, v2);

		Carbon::ShowHud = GetBit(result, 1);
		if (Global::ShowVanilla())
		{
			return result;
		}

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

		return resHud != 0 && hud && Carbon::ShowHud && resPl != 0 && pl;
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