#include "Globals.h"

using namespace std;

namespace Global
{
	string HUDPath;
	string GetHudPath()
	{
		return HUDPath;
	}

	HUD_Params HUDParams;

	D3DXVECTOR2 rndVec;
	void SetRand(bool rnd)
	{
		if (rnd)
		{
			rndVec.x = rand() % 5 + 1;
			rndVec.y = rand() % 5 + 1;
		}
		else
		{
			rndVec.x = 0;
			rndVec.y = 0;
		}
	}

	D3DXVECTOR2 GetRand()
	{
		return rndVec;
	}

	bool IsFileExist(string& path)
	{
		ifstream f(("scripts\\" + path).c_str());
		return f.good();
	}

	void Init()
	{
		CIniReader main_ini("CustomHud.ini");

		HUDPath = main_ini.ReadString((char*)"GENERAL", (char*)"HUDpath", "");
		string iniName = main_ini.ReadString((char*)"GENERAL", (char*)"IniName", "");

		string iniPath = GetHudPath() + iniName;
		if (!IsFileExist(iniPath))
		{
			MessageBoxA(NULL, ("HUD config not found: \n" + iniPath).c_str(), "NFSC - Custom HUD", MB_ICONERROR);
		}

		CIniReader ini(iniPath.c_str());

		HUDParams = HUD_Params();
		HUDParams.Scale = main_ini.ReadFloat((char*)"GENERAL", (char*)"Scale", 1.0f);
		HUDParams.Offset.x = main_ini.ReadFloat((char*)"GENERAL", (char*)"OffsetX", 0.0f);
		HUDParams.Offset.y = main_ini.ReadFloat((char*)"GENERAL", (char*)"OffsetY", 0.0f);
		HUDParams.HotReloadKey = main_ini.ReadInteger((char*)"GENERAL", (char*)"HotReloadKey", 0);
		HUDParams.ShowDebugInfo = main_ini.ReadInteger((char*)"GENERAL", (char*)"ShowDebugInfo", 0);

		HUDParams.ReplaceDragHud = main_ini.ReadInteger((char*)"MOST_WANTED", (char*)"ReplaceDragHud", 0);

		HUDParams.Init(ini);
	}
};
