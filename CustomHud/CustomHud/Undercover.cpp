#include "Undercover.h"

namespace Game
{
	Undercover::Undercover()
	{

	}

	int Undercover::Device()
	{
		return 0x00EA0110;
	}
	float Undercover::GetBoost()
	{
		return 0.0f;
	}
	bool Undercover::IsBoostInstalled()
	{
		return false;
	}
	float Undercover::GetNos()
	{
		return 0.0f;
	}
	bool Undercover::IsNosInstalled()
	{
		return false;
	}
	float Undercover::GetSpeedBreaker()
	{
		return 0.0f;
	}
	float Undercover::GetSpeed()
	{
		return 0.0f;
	}
	bool Undercover::IsHudVisible()
	{
		return true;
	}
	float Undercover::GetRPM()
	{
		return 0.0f;
	}
	float Undercover::GetRedline()
	{
		return 0.0f;
	}
	int Undercover::GetGear()
	{
		return 0;
	}
	bool Undercover::GetUnits()
	{
		return false;
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