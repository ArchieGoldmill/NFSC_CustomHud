#pragma warning( push )
#pragma warning( disable : 26495)
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "CarHud.h"
#include <chrono>
#include "Globals.h"
#include "MirrorHook/D3D9/D3D9Extender.hpp"
#include "GameApi.h"
#include "Carbon.h"
#include "MostWanted.h"
using namespace MirrorHookInternals;
using namespace std;

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

HINSTANCE DllHandle;

typedef HRESULT(__stdcall* reset)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
reset pReset;

typedef HRESULT(__stdcall* endScene)(IDirect3DDevice9* pDevice);
endScene pEndScene;

HUD* carHud = NULL;
void __stdcall hookedEndScene(IDirect3DDevice9* pDevice)
{
	if (carHud == NULL)
	{
		carHud = new HUD(pDevice);
	}

	carHud->Draw();
}

void __stdcall hookedReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (carHud != NULL)
	{
		delete carHud;
		carHud = NULL;

		Sprite::Reset();
	}
}

void Init()
{
	IDirect3DDevice9* pDevice = nullptr;
	while (pDevice == NULL)
	{
		Sleep(10);
		pDevice = *(IDirect3DDevice9 * *)Game::Current->Device();
	}

	D3D9Extender::AddExtension(D3D9Extender::D3D9Extension::EndScene, hookedEndScene);
	D3D9Extender::AddExtension(D3D9Extender::D3D9Extension::AfterReset, hookedReset);

	if (!D3D9Extender::Init(&pDevice))
	{
		MessageBoxA(NULL, "Unable to init hook.", "Custom HUD", MB_ICONERROR);
		return;
	}

	Global::Init();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		int ptr = (base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base));
		if (ptr == 0x87E926) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
		{
			Game::Current = new Game::Carbon();
		}
		else if (ptr == 0x7C4040)
		{
			Game::Current = new Game::MostWanted();
		}
		else
		{
			MessageBoxA(NULL, "This .exe is not supported.", "Custom HUD", MB_ICONERROR);
			return FALSE;
		}

		DisableThreadLibraryCalls(hModule);
		std::thread(Init).detach();
	}
	break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
