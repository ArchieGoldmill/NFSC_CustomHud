#pragma warning( push )
#pragma warning( disable : 26495 )
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "Globals.h"

#include "Underground.h"
#include "Underground2.h"
#include "MostWantedOnline.h"
#include "Carbon.h"
#include "ProStreet.h"
#include "Undercover.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "MinHook.lib")

void Init()
{
	Game::Current->Detour();
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
			if (Global::IsFileExist("scripts\\MostWantedOnline.asi"))
			{
				Game::Current = new Game::MostWantedOnline();
			}
			else
			{
				Game::Current = new Game::MostWanted();
			}
		}
		else if (ptr == 0x008aec55 || ptr == 0x014082ed)
		{
			Game::Current = new Game::Undercover();
		}
		else if (ptr == 0x0075BCC7)
		{
			Game::Current = new Game::Underground2();
		}
		else if (ptr == 0x00670CB5)
		{
			Game::Current = new Game::Underground();
		}
		else if (ptr == 0x00828c25 || ptr == 0x1aaa080)
		{
			Game::Current = new Game::ProStreet();
		}
		else
		{
			char buf[100];
			sprintf(buf, "This .exe is not supported. Ptr=%x", ptr);
			MessageBoxA(NULL, buf, "Custom HUD", MB_ICONERROR);
			return FALSE;
		}

		Global::Init();

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
