// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <map>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "detours.h"
#pragma comment(lib, "detours.lib")
#include "Types.h"
#include <DxErr.h>
#pragma comment(lib, "dxerr.lib")

using namespace std;

HINSTANCE DllHandle;

typedef HRESULT(__stdcall* endScene)(IDirect3DDevice9* pDevice);
endScene pEndScene;

map<HUD, LPDIRECT3DTEXTURE9> Textures;
LPDIRECT3DTEXTURE9 GetTexture(IDirect3DDevice9* pDevice, HUD hudPart)
{
	LPDIRECT3DTEXTURE9 pTexture = nullptr;
	for (auto& x : Textures)
	{
		if (x.first == hudPart)
		{
			return x.second;
		}
	}

	if (hudPart == HUD::Background)
	{
		if (FAILED(D3DXCreateTextureFromFile(pDevice, "scripts\\CustomHUD\\background.dds", &pTexture)))
		{
			return nullptr;
		}
	}

	Textures.insert({ hudPart, pTexture });

	return pTexture;
}

map<HUD, ID3DXSprite*> Sprites;
void DrawSprite(IDirect3DDevice9* pDevice, HUD hudPart)
{
	ID3DXSprite* pSprite = nullptr;
	for (auto& x : Sprites)
	{
		if (x.first == hudPart)
		{
			pSprite = x.second;
		}
	}

	if (pSprite == nullptr)
	{
		if (!FAILED(D3DXCreateSprite(pDevice, &pSprite)))
		{
			Sprites.insert({ hudPart, pSprite });
		}
	}

	if (pSprite != nullptr)
	{
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 100;
		rect.bottom = 100;

		D3DXVECTOR3 center;
		center.x = 0;
		center.y = 0;
		center.z = 0;

		D3DXVECTOR3 position;
		position.x = 100;
		position.y = 100;

		D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);

		//if (FAILED(pSprite->Draw(pTexture, &rect, &center, &position, color)))
		auto pTexture = GetTexture(pDevice, hudPart);
		if (pTexture != nullptr)
		{
			pSprite->Begin(D3DXSPRITE_ALPHABLEND);
			HRESULT hr = pSprite->Draw(pTexture, NULL, NULL, NULL, color);
			const char* a= DXGetErrorString(hr);
			const char* b = DXGetErrorDescription(hr);
			if (FAILED(hr))
			{
				center.x = 1;
			}
			pSprite->End();
		}
	}
}

HRESULT __stdcall hookedEndScene(IDirect3DDevice9* pDevice)
{
	DrawSprite(pDevice, HUD::Background);

	return pEndScene(pDevice); // call original endScene
}

void hookEndScene()
{
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION); // create IDirect3D9 object
	if (!pD3D)
		return;

	D3DPRESENT_PARAMETERS d3dparams = { 0 };
	d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dparams.hDeviceWindow = GetForegroundWindow();
	d3dparams.Windowed = true;

	IDirect3DDevice9* pDevice = nullptr;

	HRESULT result = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dparams.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dparams, &pDevice);
	if (FAILED(result) || !pDevice) {
		pD3D->Release();
		return;
	}
	//if device creation worked out -> lets get the virtual table:
	void** vTable = *reinterpret_cast<void***>(pDevice);

	//now detour:

	pEndScene = (endScene)DetourFunction((PBYTE)vTable[42], (PBYTE)hookedEndScene);

	pDevice->Release();
	pD3D->Release();
}


DWORD __stdcall EjectThread(LPVOID lpParameter)
{
	Sleep(100);
	FreeLibraryAndExitThread(DllHandle, 0);
	return 0;
}


DWORD WINAPI Menue(HINSTANCE hModule)
{

	hookEndScene();

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
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Menue, NULL, 0, NULL);
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
