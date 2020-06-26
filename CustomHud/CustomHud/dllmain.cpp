#pragma warning( push )
#pragma warning( disable : 26495)
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "injector/injector.hpp"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "detours.h"
#pragma comment(lib, "detours.lib")
#include "CarHud.h"
#include <chrono>
#include "Globals.h"
#include "MirrorHook/D3D9/D3D9Extender.hpp"
using namespace MirrorHookInternals;

using namespace std;

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
		Global::Init();
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
	}
}

auto Game_DetermineHudFeatures = (int(__thiscall*)(void* _this, signed int var1))0x005DC4B0;

bool GetBit(int n, int k)
{
	return (n & (1 << k)) >> k;
}

void ClearBit(int& n, int k)
{
	n &= ~(1 << k);
}

int __fastcall DetermineHudFeatures(void* _this, int v1, int v2)
{
	int result = Game_DetermineHudFeatures(_this, v2);

	HUD::ShowHUD = GetBit(result, 1);

	ClearBit(result, 1);
	ClearBit(result, 11);
	//ClearBit(result, 14); // Heat bar
	ClearBit(result, 18);

	return result;
}

void Init()
{
	IDirect3DDevice9* pDevice = nullptr;
	while (pDevice == NULL)
	{
		Sleep(10);
		pDevice = *(IDirect3DDevice9 * *)0xAB0ABC;
	}

	D3D9Extender::AddExtension(D3D9Extender::D3D9Extension::EndScene, hookedEndScene);
	D3D9Extender::AddExtension(D3D9Extender::D3D9Extension::AfterReset, hookedReset);

	if (D3D9Extender::Init(&pDevice))
	{
		injector::MakeCALL(0x005E6F7B, DetermineHudFeatures, true);
	}
	else
	{
		MessageBoxA(NULL, "Unable to init hook.", "NFSC - Custom HUD", MB_ICONERROR);
	}
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

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x87E926) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
		{
			DisableThreadLibraryCalls(hModule);
			std::thread(Init).detach();
		}
		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English nfsc.exe (6,88 MB (7.217.152 bytes)).", "NFSC - Custom HUD", MB_ICONERROR);
			return FALSE;
		}
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
