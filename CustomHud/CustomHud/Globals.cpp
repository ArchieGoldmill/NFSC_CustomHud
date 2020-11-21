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

	bool IsInit = false;

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
		if (IsInit)
		{
			return;
		}

		CIniReader main_ini("CustomHud.ini");

		HUDPath = main_ini.ReadString((char*)"GENERAL", (char*)"HUDpath", "");

		string iniPath = GetHudPath().append("hud.ini");
		if (!IsFileExist(iniPath))
		{
			MessageBoxA(NULL, ("HUD conig not found: \n" + iniPath).c_str(), "NFSC - Custom HUD", MB_ICONERROR);
		}

		CIniReader ini(iniPath.c_str());

		HUDParams.Scale = main_ini.ReadFloat((char*)"GENERAL", (char*)"Scale", 1.0f);
		HUDParams.Offset.x = main_ini.ReadFloat((char*)"GENERAL", (char*)"OffsetX", 0.0f);
		HUDParams.Offset.y = main_ini.ReadFloat((char*)"GENERAL", (char*)"OffsetY", 0.0f);
		HUDParams.Init(ini);

		IsInit = true;
	}
};
