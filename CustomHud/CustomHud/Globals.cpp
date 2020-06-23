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

	void Init()
	{
		if (IsInit)
		{
			return;
		}

		CIniReader main_ini("CustomHud.ini");

		HUDPath = main_ini.ReadString((char*)"GENERAL", (char*)"HUDpath", "");

		CIniReader ini(GetHudPath().append("hud.ini").c_str());

		HUDParams.Scale = main_ini.ReadFloat((char*)"GENERAL", (char*)"Scale", 1.0f);
		HUDParams.Init(ini);

		IsInit = true;
	}
};
