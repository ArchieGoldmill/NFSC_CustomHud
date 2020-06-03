// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "injector/injector.hpp"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "detours.h"
#pragma comment(lib, "detours.lib")
#include "CarHud.h"

using namespace std;

HINSTANCE DllHandle;

typedef HRESULT(__stdcall* reset)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
reset pReset;

typedef HRESULT(__stdcall* endScene)(IDirect3DDevice9* pDevice);
endScene pEndScene;

CarHud* carHud = NULL;
HRESULT __stdcall hookedEndScene(IDirect3DDevice9* pDevice)
{
	if (carHud == NULL)
	{
		carHud = new CarHud(pDevice, "CustomHUD");
	}

	carHud->Draw();

	return pEndScene(pDevice);
}

HRESULT __stdcall hookedReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (carHud != NULL)
	{
		delete carHud;
		carHud = NULL;
	}

	TextureInfo::Release();

	return pReset(pDevice, pPresentationParameters);
}

bool hookEndScene()
{
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION); // create IDirect3D9 object
	if (!pD3D) 
	{
		return false;
	}

	D3DPRESENT_PARAMETERS d3dparams = { 0 };
	d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dparams.hDeviceWindow = GetForegroundWindow();
	d3dparams.Windowed = true;

	IDirect3DDevice9* pDevice = nullptr;

	HRESULT result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dparams.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dparams, &pDevice);
	if (FAILED(result) || !pDevice) {
		pD3D->Release();
		return false;
	}
	//if device creation worked out -> lets get the virtual table:
	void** vTable = *reinterpret_cast<void***>(pDevice);

	//now detour:

	pEndScene = (endScene)DetourFunction((PBYTE)vTable[42], (PBYTE)hookedEndScene);
	pReset = (reset)DetourFunction((PBYTE)vTable[16], (PBYTE)hookedReset);

	pDevice->Release();
	pD3D->Release();

	return true;
}


DWORD __stdcall EjectThread(LPVOID lpParameter)
{
	Sleep(100);
	FreeLibraryAndExitThread(DllHandle, 0);
	return 0;
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

	CarHud::ShowHUD = GetBit(result, 1);

	ClearBit(result, 1);
	ClearBit(result, 11);
	ClearBit(result, 14);
	ClearBit(result, 18);

	return result;
}

DWORD WINAPI Init(HINSTANCE hModule)
{
	if (hookEndScene())
	{
		injector::MakeCALL(0x005E6F7B, DetermineHudFeatures, true);
	}

	return 0;
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
			//hookEndScene();
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Init, NULL, 0, NULL);
		}
		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English nfsc.exe (6,88 MB (7.217.152 bytes)).", "NFSC - Customization Extender", MB_ICONERROR);
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
