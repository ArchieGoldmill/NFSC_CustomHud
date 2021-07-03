#pragma once
#include "HUD_Element.h"
#include "HUD_Digit.h"
#include "Globals.h"
#include "GameApi.h"

class HUD_Speedometer : HUD_Element
{
private:
	HUD_Speedometer_Params params;

	HUD_Digit* Speed[3];


public:
	HUD_Speedometer(LPDIRECT3DDEVICE9 pDevice, HUD_Speedometer_Params params) : HUD_Element(pDevice)
	{
		this->params = params;

		for (int i = 0; i < 3; i++)
		{
			HUD_Digit_Params digitParams = this->params;

			IntValueCallback* cb = 0;
			if (i == 0)
			{
				cb = Game::GetSpeed0;
			}
			else if (i == 1)
			{
				cb = Game::GetSpeed1;
			}
			else if (i == 2)
			{
				cb = Game::GetSpeed2;
			}

			digitParams.GetNumber = cb;

			digitParams.Position.y = this->params.Position.y;

			this->Speed[i] = new HUD_Digit(pDevice, digitParams);
		}
	}

	void Draw()
	{
		int numCount = 3;
		int speed = Game::GetSpeed();
		if (this->params.DigitsMode == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				this->Speed[i]->SetPositionX(this->params.Position.x + i * this->params.Size + this->params.Distance * i);
			}
		}
		else
		{
			if (speed < 10)
			{
				numCount = 1;
			}
			else if (speed < 100) {
				numCount = 2;
			}

			if (this->params.DigitsMode == 1)
			{
				for (int i = 0; i < numCount; i++)
				{
					this->Speed[i]->SetPositionX(this->params.Position.x + i * this->params.Size + this->params.Distance * i);
				}
			}
			else if (this->params.DigitsMode == 2)
			{
				for (int i = 0; i < numCount; i++)
				{
					this->Speed[i]->SetPositionX((this->params.Position.x + i * this->params.Size + this->params.Distance * i) +
						((this->params.Size + this->params.Distance) / 2 * (3 - numCount)));
				}
			}
		}

		for (int i = 0; i < numCount; i++)
		{
			int digit = speed / (int)pow(10, i);
			this->Speed[i]->Draw(digit != 0 || i == 0);
		}
	}

	void Release()
	{
		if (!this->isReleased)
		{
			for (int i = 0; i < 3; i++)
			{
				if (this->Speed[i] != NULL)
				{
					delete this->Speed[i];
				}
			}

			this->isReleased = true;
		}
	}

	~HUD_Speedometer()
	{
		this->Release();
	}
};
