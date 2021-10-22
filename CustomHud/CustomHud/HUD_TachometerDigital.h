#pragma once
#include "HUD_Element.h"
#include "HUD_Digit.h"
#include "HUD_Digital.h"
#include "GameApi.h"

class HUD_TachometerDigital : public HUD_Digital
{
private:
	HUD_Digital_Params params;

	HUD_Digit* Speed[4];

public:
	HUD_TachometerDigital(LPDIRECT3DDEVICE9 pDevice, HUD_Digital_Params params) : HUD_Digital(pDevice)
	{
		this->params = params;

		for (int i = 0; i < 4; i++)
		{
			HUD_Digit_Params digitParams = this->params;

			IntValueCallback* cb = 0;
			switch (i)
			{
			case 0: cb = Game::GetRPM0; break;
			case 1: cb = Game::GetRPM1; break;
			case 2: cb = Game::GetRPM2; break;
			case 3: cb = Game::GetRPM3; break;
			}

			digitParams.GetNumber = cb;
			digitParams.Position.y = this->params.Position.y;

			this->Speed[i] = new HUD_Digit(pDevice, digitParams);
		}
	}

	void Draw()
	{
		int num = 4;
		if (this->params.DigitsMode == 1)
		{
			num = this->GetNumDigits(Game::GetRPM() * 1000);
		}

		for (int i = 0; i < num; i++)
		{
			this->Speed[i]->SetPositionX(this->params.Position.x + i * this->params.Size + this->params.Distance * i);
			this->Speed[i]->Draw();
		}
	}

	void Release()
	{
		if (!this->isReleased)
		{
			for (int i = 0; i < 4; i++)
			{
				if (this->Speed[i] != NULL)
				{
					delete this->Speed[i];
				}
			}

			this->isReleased = true;
		}
	}

	~HUD_TachometerDigital()
	{
		this->Release();
	}
};