#include "BaseGame.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "MirrorHook/D3D9/D3D9Extender.hpp"
#include "Hooks.h"
#include "Globals.h"

using namespace MirrorHookInternals;

namespace Game
{
	bool BaseGame::ShowHud = 0;

	void BaseGame::Detour()
	{
		IDirect3DDevice9* pDevice = NULL;
		while (pDevice == NULL)
		{
			Sleep(10);
			pDevice = *(IDirect3DDevice9**)Game::Current->Device();
		}

		D3D9Extender::AddExtension(D3D9Extender::D3D9Extension::EndScene, hookedEndScene);
		D3D9Extender::AddExtension(D3D9Extender::D3D9Extension::BeforeReset, hookedReset);

		if (!D3D9Extender::Init(&pDevice))
		{
			MessageBoxA(NULL, "Unable to init hook.", "Custom HUD", MB_ICONERROR);
		}
	}

	bool BaseGame::IsSpeedBreakerInstalled()
	{
		return true;
	}
}