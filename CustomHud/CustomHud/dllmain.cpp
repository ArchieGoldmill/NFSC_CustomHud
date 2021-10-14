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
#include "Undercover.h"
using namespace MirrorHookInternals;
using namespace std;

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

ID3DXFont* Font = NULL;
void DrawTextS(string str, int line, IDirect3DDevice9* pDevice, int color = 0xFFFFFFFF)
{
	if (Font == NULL)
	{
		D3DXCreateFont(pDevice, 22, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &Font);
	}

	RECT font_rect;
	font_rect.left = 10;
	font_rect.top = 30 * line;
	font_rect.right = 100;
	font_rect.bottom = 30 * (line + 1);

	Font->DrawText(NULL, str.c_str(), -1, &font_rect, DT_LEFT | DT_NOCLIP, color);
}

template <typename T> string tostr(const T& t) {
	ostringstream os;
	os << t;
	return os.str();
}

void DrawDebugInfo(int hudDrawText, IDirect3DDevice9* pDevice)
{
	DrawTextS("RPM=" + std::to_string(Game::GetRPM() * 1000.0f), 0, pDevice);
	DrawTextS("RedLine=" + std::to_string(Game::GetRedline() * 1000.0f), 1, pDevice);
	DrawTextS("Speed=" + std::to_string(Game::GetSpeed()), 2, pDevice);
	DrawTextS("NOS=" + std::to_string(Game::GetNos()), 3, pDevice);
	DrawTextS("SpeedBreaker=" + std::to_string(Game::GetSpeedBreaker()), 4, pDevice);
	DrawTextS("Boost=" + std::to_string(Game::GetBoost()), 5, pDevice);
	DrawTextS("TIME(microsec)=" + std::to_string(hudDrawText), 6, pDevice);
	DrawTextS("DeltaTime=" + tostr(Global::DeltaTime), 7, pDevice);
}

HUD* carHud = NULL;
void __stdcall hookedReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (Font)
	{
		Font->Release();
		Font = NULL;
	}

	if (carHud)
	{
		delete carHud;
		carHud = NULL;
		Sprite::Reset();
	}
}

auto start = chrono::steady_clock::now();
void __stdcall hookedEndScene(IDirect3DDevice9* pDevice)
{
	if (!Game::Current->IsHudVisible())
	{
		return;
	}

	auto hudDrawStart = chrono::steady_clock::now();

	if ((GetAsyncKeyState(Global::HUDParams.HotReloadKey) & 1))
	{
		hookedReset(NULL, NULL);
		Global::Init();
	}
	else if (Global::HUDParams.CustomCarHUDs)
	{
		string carName = Game::Current->GetCarName();

		if (!carName.empty() && carName != Global::CurrentCar)
		{
			Global::CurrentCar = carName;
			bool newCarHasHud = Global::CarHasHud(carName);
			if (newCarHasHud || Global::CarHasHUD != newCarHasHud)
			{
				Global::CarHasHUD = newCarHasHud;
				hookedReset(NULL, NULL);

				if (!Global::ShowVanilla())
				{
					Global::Init();
				}
			}
		}
	}

	if (!carHud && !Global::ShowVanilla())
	{
		carHud = new HUD(pDevice);
	}

	if (carHud)
	{
		carHud->Draw();
	}

	auto now = chrono::steady_clock::now();
	Global::DeltaTime = chrono::duration_cast<std::chrono::microseconds>(now - start).count() / 1000.0f;
	start = chrono::steady_clock::now();

	if (Global::HUDParams.ShowDebugInfo)
	{
		int hudDrawTime = chrono::duration_cast<std::chrono::microseconds>(now - hudDrawStart).count();
		DrawDebugInfo(hudDrawTime, pDevice);
	}

	//DrawTextS("TEST UC CUSTOM HUD - BY ARCHIE / NOT FOR PUBLIC USE", 0, pDevice);
	//DrawTextS("TEST UC CUSTOM HUD - BY ARCHIE / NOT FOR PUBLIC USE", 1, pDevice, 0xFF000000);
}

void Init()
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
		//else if (ptr == 0x008aec55 || ptr == 0x014082ed)
		//{
		//	Game::Current = new Game::Undercover();
		//}
		else
		{
			char buf[100];
			sprintf(buf, "This .exe is not supported. Ptr=%x", ptr);
			MessageBoxA(NULL, buf, "Custom HUD", MB_ICONERROR);
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
