#include "GameApi.h"
#include <vector>

namespace Game
{
	Api* Current;

	float GetBoost()
	{
		float res = Current->GetBoost();

		int max = 20.0f;

		if (res > max)
		{
			res = max;
		}

		if (res < -max)
		{
			res = -max;
		}

		return res;
	}

	bool IsBoostInstalled()
	{
		return Current->IsBoostInstalled();
	}

	float GetNos()
	{
		return Current->GetNos();
	}

	bool IsNosInstalled()
	{
		return Current->IsNosInstalled();
	}

	float GetSpeedBreaker()
	{
		return Current->GetSpeedBreaker();
	}

	float GetSpeed()
	{
		float res = Current->GetSpeed();

		if (res > 999.0f)
		{
			res = 999.0f;
		}

		return abs(res);
	}

	bool IsHudVisible()
	{
		return Current->IsHudVisible();
	}

	float GetRPM()
	{
		float res = Current->GetRPM();

		return res / 1000.0f;
	}

	float GetRedline()
	{
		float res = Current->GetRedline() - 500.0f;

		return res / 1000.0f;
	}

	int GetGear()
	{
		int gear = Current->GetGear();

		if (gear == 0)
		{
			gear = 12;
		}

		if (gear == 1)
		{
			gear = 11;
		}

		if (gear > 12 || gear < 0)
		{
			gear = 0;
		}

		return gear;
	}

	bool GetUnits()
	{
		return Current->GetUnits();
	}

	bool IsInPerfectLaunchRange()
	{
		return Current->IsInPerfectLaunchRange();
	}

	int GetSpeed0()
	{
		return (int)GetSpeed() % 10 + 1;
	}

	int GetSpeed1()
	{
		return (int)GetSpeed() / 10 % 10 + 1;
	}

	int GetSpeed2()
	{
		return (int)GetSpeed() / 100 % 10 + 1;
	}
}

int* GetPtr(std::vector<int>& offsets)
{
	int* ptr = (int*)offsets[0];
	for (int i = 1, size = offsets.size(); i < size; i++)
	{
		if (ptr == NULL || *ptr == 0)
		{
			return 0;
		}

		ptr = (int*)(*ptr + offsets[i]);
	}

	return ptr;
}

bool GetBit(int n, int k)
{
	return (n & (1 << k)) >> k;
}

void ClearBit(int& n, int k)
{
	n &= ~(1 << k);
}