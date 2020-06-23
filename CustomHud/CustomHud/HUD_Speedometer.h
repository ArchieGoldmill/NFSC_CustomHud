#pragma once
#include "HUD_Element.h"
#include "HUD_Digit.h"
#include "DALVehicleCarbon.h"
#include "Globals.h"

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

		if (!this->params.Units.TextureUnits.empty())
		{
			this->units = new Sprite(pDevice, this->params.Units.TextureUnits);
		}

		for (int i = 0; i < 3; i++)
		{
			HUD_Digit_Params digitParams = this->params;

			IntValueCallback* cb = 0;
			if (i == 0)
			{
				cb = GetSpeed0;
			}
			if (i == 1)
			{
				cb = GetSpeed1;
			}
			if (i == 2)
			{
				cb = GetSpeed2;
			}

			digitParams.GetNumber = cb;

			digitParams.Position.x = this->params.Position.x + i * (digitParams.Size / 1.55f) + this->params.Distance * i;
			digitParams.Position.y = this->params.Position.y;

			this->Speed[i] = new HUD_Digit(pDevice, digitParams);
		}
	}

	void Draw()
	{
		int numCount = 3;
		if (!this->params.ShowAllDigits)
		{
			int speed = GetSpeed();
			if (speed < 10)
			{
				numCount = 1;
			}
			else if (speed < 100) {
				numCount = 2;
			}
		}

		for (int i = 0; i < numCount; i++)
		{
			this->Speed[i]->Draw();
		}

		if (this->units != NULL)
		{
			RECT rect;
			if (IsKMH())
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

			this->Setup(this->units, { this->params.Units.Size * 4, this->params.Units.Size }, { 0, 0 }, this->params.Units.Position, &rect, 0);
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
					this->Speed[i]->Release();

					delete this->Speed[i];
				}
			}

			if (this->units != NULL)
			{
				this->units->Release();

				delete this->units;
			}

			this->isReleased = true;
		}
	}

	~HUD_Speedometer()
	{
		this->Release();

		for (int i = 0; i < 3; i++)
		{
			delete this->Speed[i];
		}
	}
};
