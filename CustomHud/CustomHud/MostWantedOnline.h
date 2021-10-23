#pragma once
#include <string>
#include "MostWanted.h"

namespace Game
{
	class MostWantedOnline : public MostWanted
	{
	public:
		MostWantedOnline();

		void Detour();
		bool IsSpeedBreakerInstalled();
	};
}