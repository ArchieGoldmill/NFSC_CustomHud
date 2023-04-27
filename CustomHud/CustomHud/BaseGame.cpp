#include "BaseGame.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "Hooks.h"
#include "Globals.h"
#include "MinHook.h"

namespace Game
{
	void BaseGame::Detour()
	{
		IDirect3DDevice9* pDevice = NULL;
		while (pDevice == NULL)
		{
			Sleep(1000);
			auto device = (IDirect3DDevice9**)Game::Current->Device();
			if (device)
			{
				pDevice = *device;
			}
		}

		void** vt = *(void***)pDevice;

		MH_Initialize();

		MH_CreateHook(vt[16], hookedReset, (void**)&oReset);
		MH_CreateHook(vt[42], hookedEndScene, (void**)&oEndScene);

		MH_EnableHook(MH_ALL_HOOKS);
	}

	float BaseGame::GetShake()
	{
		return 0;
	}

	bool BaseGame::IsSpeedBreakerInstalled()
	{
		return true;
	}

	bool BaseGame::IsNosCountInstalled()
	{
		return false;
	}

	int BaseGame::GetNosCount()
	{
		return 0;
	}
}