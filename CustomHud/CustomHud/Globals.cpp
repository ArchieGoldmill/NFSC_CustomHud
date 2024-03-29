#include "Globals.h"

using namespace std;

namespace Global
{
	std::string HUDPath;
	std::string GetHudPath()
	{
		return HUDPath;
	}

	HUD_Params HUDParams;
	std::string CurrentCar;
	bool CarHasHUD = false;
	float DeltaTime;

	bool IsFileExist(std::string path)
	{
		char buffer[MAX_PATH];
		HMODULE hm = NULL;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, NULL, &hm);
		GetModuleFileNameA(hm, buffer, sizeof(buffer));

		std::string modulePath = buffer;
		path = modulePath.substr(0, modulePath.rfind('\\') + 1) + path;

		ifstream f(path.c_str());
		bool isGood = f.good();
		f.close();

		return isGood;
	}

	std::string GetCarHudPath(std::string name)
	{
		return "CARS\\" + name + "\\CustomHUD\\hud.ini";
	}

	bool CarHasHud(std::string name)
	{
		if (!name.empty())
		{
			return IsFileExist(GetCarHudPath(name));
		}

		return false;
	}

	bool ShowVanilla()
	{
		return Global::HUDParams.CustomCarHUDs == 2 && !Global::CarHasHUD;
	}

	void Init()
	{
		CIniReader main_ini("scripts\\CustomHud.ini");

		HUDParams = HUD_Params();
		HUDParams.Scale = main_ini.ReadFloat((char*)"GENERAL", (char*)"Scale", 1.0f);
		HUDParams.Offset.x = main_ini.ReadFloat((char*)"GENERAL", (char*)"OffsetX", 0.0f);
		HUDParams.Offset.y = main_ini.ReadFloat((char*)"GENERAL", (char*)"OffsetY", 0.0f);
		HUDParams.HotReloadKey = main_ini.ReadInteger((char*)"GENERAL", (char*)"HotReloadKey", 0);
		HUDParams.ShowDebugInfo = main_ini.ReadInteger((char*)"GENERAL", (char*)"ShowDebugInfo", 0);
		HUDParams.CustomCarHUDs = main_ini.ReadInteger((char*)"GENERAL", (char*)"CustomCarHUDs", 0);

		HUDParams.ShakeAmount = main_ini.ReadFloat((char*)"CARBON", (char*)"ShakeAmount", 0);

		std::string iniPath;
		if (HUDParams.CustomCarHUDs && CarHasHUD)
		{
			HUDPath = "CARS\\" + CurrentCar + "\\CustomHUD\\";
			iniPath = HUDPath + "hud.ini";
		}
		else
		{
			auto hudPathStr = main_ini.ReadString((char*)"GENERAL", (char*)"HUDpath", "");
			HUDPath = "scripts\\" + hudPathStr;

			std::string iniName = main_ini.ReadString((char*)"GENERAL", (char*)"IniName", "");
			iniPath = GetHudPath() + iniName;
			if (!IsFileExist(iniPath))
			{
				MessageBoxA(NULL, ("HUD config not found: \n" + iniPath).c_str(), "Custom HUD", MB_ICONERROR);
			}
		}

		CIniReader ini(iniPath.c_str());
		HUDParams.Init(ini);
	}
};
