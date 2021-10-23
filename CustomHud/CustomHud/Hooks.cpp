#include "Hooks.h"
#include "CarHud.h"

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
}