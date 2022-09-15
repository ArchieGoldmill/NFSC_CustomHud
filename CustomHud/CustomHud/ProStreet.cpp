#include "ProStreet.h"
#include "injector/injector.hpp"

namespace Game::ProStreetApi
{
	auto DALVehicle_GetGear = (bool(__stdcall*)(int* getVal, const int playerNum))0x0055B380;
	auto DALVehicle_GetInPerfectLaunchRange = (bool(__stdcall*)(int* getVal, const int playerNum))0x0055B670;
	auto DALVehicle_GetIsPlayerControlling = (bool(__stdcall*)(int* getVal, const int playerNum))0x0055B7F0;
	auto DALVehicle_GetIsHudVisible = (bool(__stdcall*)(int* getVal, const int playerNum))0x005514F0;

	auto DALVehicle_GetRPM = (bool(__stdcall*)(float* getVal, const int playerNum))0x0055AFD0;
	auto DALVehicle_GetRedline = (bool(__stdcall*)(float* getVal, const int playerNum))0x0055B080;
	auto DALVehicle_GetBoost = (bool(__stdcall*)(float* getVal, const int playerNum))0x0055B2A0;
	auto DALVehicle_GetSpeed = (bool(__stdcall*)(float* getVal, const int playerNum))0x0055AED0;

	auto GetRaceType = (int(__thiscall*)(int))0x00404CE0;

	bool IsDragMode()
	{
		int raceStatus = *(int*)0x00AB9D7C;
		if (raceStatus)
		{
			return GetRaceType(raceStatus) == 3;
		}

		return false;
	}

	bool IsHudEnabled = false;
	bool IsNosInstalled = false;
	bool IsBoostInstalled = false;
	auto Game_DetermineHudFeatures = (unsigned int(__thiscall*)(int, int))0x0061C0B0;
	int __fastcall DetermineHudFeatures(int a1, int, int a2)
	{
		auto result = Game_DetermineHudFeatures(a1, a2);
		IsHudEnabled = false;

		if (!IsDragMode())
		{
			IsHudEnabled = GetBit(result, 0);
			IsNosInstalled = GetBit(result, 9);
			IsBoostInstalled = GetBit(result, 10);

			ClearBit(result, 0); // Tachometer
			ClearBit(result, 9); // Nos
			ClearBit(result, 10); // Boost
		}

		return result;
	}

	struct Nos
	{
		float Actual;
		float unk;
		float Fill;
		int Count;
	};

	Nos* GetNos()
	{
		unsigned int offsets[] = { 0x00ACE110, 0x40, 0xB4 };
		return (Nos*)GetPtr(offsets, 3);
	}
}

namespace Game
{
	ProStreet::ProStreet()
	{
		injector::MakeCALL(0x0061DDC3, ProStreetApi::DetermineHudFeatures, true);
	}

	int ProStreet::Device()
	{
		return *(int*)0x00AC6ED4;
	}

	float ProStreet::GetBoost()
	{
		float boost = 0;
		ProStreetApi::DALVehicle_GetBoost(&boost, 0);
		return boost;
	}

	bool ProStreet::IsBoostInstalled()
	{
		return ProStreetApi::IsBoostInstalled;
	}

	float ProStreet::GetNos()
	{
		auto nos = ProStreetApi::GetNos();
		if (nos)
		{
			if (nos->Count == 0)
			{
				return nos->Actual;
			}

			if (nos->Actual == 0)
			{
				return 1 - nos->Fill;
			}

			if (nos->Fill == 1)
			{
				return nos->Actual;
			}

			return 1;
		}

		return 0;
	}

	int ProStreet::GetNosCount()
	{
		auto nos = ProStreetApi::GetNos();
		if (nos)
		{
			return nos->Count + 1;
		}

		return 0;
	}

	bool ProStreet::IsNosCountInstalled()
	{
		return true;
	}


	bool ProStreet::IsNosInstalled()
	{
		return ProStreetApi::IsNosInstalled;
	}

	float ProStreet::GetSpeedBreaker()
	{
		return 0.0f;
	}

	float ProStreet::GetSpeed()
	{
		float speed = 0;
		ProStreetApi::DALVehicle_GetSpeed(&speed, 0);
		return speed;
	}

	bool ProStreet::IsHudVisible()
	{
		int control = 0;
		ProStreetApi::DALVehicle_GetIsPlayerControlling(&control, 0);

		int hud = 0;
		ProStreetApi::DALVehicle_GetIsHudVisible(&hud, 0);

		return ProStreetApi::IsHudEnabled && control != 0 && hud != 0;
	}

	float ProStreet::GetRPM()
	{
		float rpm = 0;
		ProStreetApi::DALVehicle_GetRPM(&rpm, 0);
		return rpm;
	}

	float ProStreet::GetRedline()
	{
		float redline = 0;
		ProStreetApi::DALVehicle_GetRedline(&redline, 0);
		return redline;
	}

	int ProStreet::GetGear()
	{
		int gear = 0;
		ProStreetApi::DALVehicle_GetGear(&gear, 0);
		return gear;
	}

	bool ProStreet::GetUnits()
	{
		unsigned int offsets[] = { 0x00AB0EAC, 0x90924 };
		auto ptr = (BYTE*)GetPtr(offsets, 2);
		if (ptr)
		{
			return *ptr;
		}

		return 0;
	}

	bool ProStreet::IsInPerfectLaunchRange()
	{
		int range = 0;
		ProStreetApi::DALVehicle_GetInPerfectLaunchRange(&range, 0);
		return range;
	}

	bool ProStreet::IsSpeedBreakerInstalled()
	{
		return false;
	}

	std::string ProStreet::GetCarName()
	{
		if (this->IsHudVisible())
		{
			unsigned int offsets[] = { 0x00ACE110, 0x54, 0x20, 0x14, 0x28, 0x40, 0x3F4, 0x40 };
			auto ptr = (char*)GetPtr(offsets, 8);
			if (ptr)
			{
				return ptr;
			}
		}

		return std::string();
	}
}