#include "MostWantedOnline.h"
#include "Hooks.h"
#include "injector/injector.hpp"

#define SAVE_REGS __asm\
{\
	__asm push ebx\
	__asm push ecx\
	__asm push edi\
	__asm push esi\
}\

#define RESTORE_REGS __asm\
{\
	__asm pop esi\
	__asm pop edi\
	__asm pop ecx\
	__asm pop ebx\
}\

namespace Game::MostWantedOnlineApi
{
	void __declspec(naked) EndSceneCave()
	{
		static constexpr auto cExit = 0x006E75BD;

		__asm
		{
			pushad;
			push eax;
			call hookedEndScene;
			popad;

			push eax;
			call dword ptr[edx + 0xA8];
			jmp cExit;
		}
	}

	void __declspec(naked) ResetCave()
	{
		static constexpr auto cExit = 0x006DB16D;

		__asm
		{
			pushad;
			push 0x0093E82C;
			push eax;
			call hookedReset;
			popad;

			push 0x0093E82C;
			push eax;
			call dword ptr[ecx + 0x40];

			jmp cExit;
		}
	}

	void __declspec(naked) DetermineHudFeaturesCave()
	{
		static constexpr auto cExit = 0x0058CA5A;

		__asm
		{
			SAVE_REGS;
			push edx;
			push eax;
			call MostWantedApi::AfterDetermineHudFeatures;
			RESTORE_REGS;

			mov edi, eax;
			cmp edi, [esi + 0x18];

			jmp cExit;
		}
	}
}

namespace Game
{
	MostWantedOnline::MostWantedOnline()
	{
		injector::MakeJMP(0x0058CA55, MostWantedOnlineApi::DetermineHudFeaturesCave, true);
		this->HookHideEverySingleHud();
	}

	void MostWantedOnline::Detour()
	{
		injector::MakeJMP(0x006E75B6, MostWantedOnlineApi::EndSceneCave, true);
		injector::MakeJMP(0x006DB164, MostWantedOnlineApi::ResetCave, true);
	}

	bool MostWantedOnline::IsSpeedBreakerInstalled()
	{
		return false;
	}
}
