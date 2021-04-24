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
	Sprite* units = NULL;

public:
	HUD_Speedometer(LPDIRECT3DDEVICE9 pDevice, HUD_Speedometer_Params params) : HUD_Element(pDevice)
	{
		this->params = params;

		if (!this->params.Units.UnitsTexture.empty())
		{
			this->units = new Sprite(pDevice, this->params.Units.UnitsTexture, this->params.Units.UnitsTextureBlendMode, D3DXVECTOR2(0, 0));
		}

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
		this->DrawSpeedometer();

		this->DrawUnits();
	}

	void DrawSpeedometer()
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

	void DrawUnits()
	{
		if (this->units != NULL)
		{
			RECT rect;
			if (Game::GetUnits())
			{
				rect.left = this->units->Info.Width / 2.0;
				rect.right = this->units->Info.Width;
			}
			else
			{
				rect.left = 0;
				rect.right = this->units->Info.Width / 2.0;
			}

			rect.top = 0;
			rect.bottom = this->units->Info.Height;

			D3DXVECTOR2 size;
			size.x = this->params.Units.Size * 2;
			size.y = this->params.Units.Size;

			D3DXVECTOR2 texSize = size;
			texSize.x *= 2;

			this->Setup(this->units, texSize, { 0, 0 }, this->params.Units.Position, &size, 0);
			this->units->Draw(&rect, this->params.Units.Color);
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

			if (this->units != NULL)
			{
				delete this->units;
			}

			this->isReleased = true;
		}
	}

	~HUD_Speedometer()
	{
		this->Release();
	}
};
