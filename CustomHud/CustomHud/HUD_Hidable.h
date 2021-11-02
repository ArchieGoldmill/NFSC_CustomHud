#pragma once
#include "Globals.h"

class HUD_Hidable
{
private:
	float opacity;
	float maxValue;
	FloatValueCallback* getValue;

public:
	HUD_Hidable(FloatValueCallback* getValue, bool maxStartValue, float maxValue)
	{
		this->getValue = getValue;
		this->opacity = maxStartValue ? maxValue : 0;
		this->maxValue = maxValue;
	}

	void CalculateOpacity()
	{
		if (!this->getValue)
		{
			return;
		}

		float value = this->getValue();
		float offset = Global::DeltaTime * 3;

		if (value >= this->maxValue || value == 0)
		{
			this->opacity -= offset;
			if (this->opacity < 0)
			{
				this->opacity = 0;
			}
		}
		else
		{
			this->opacity += offset;
			if (this->opacity > 1)
			{
				this->opacity = 1;
			}
		}
	}

	D3DCOLOR SetOpacity(D3DCOLOR color)
	{
		_D3DCOLOR colors;
		colors.Color = color;

		if (this->opacity != this->maxValue)
		{
			colors.Bytes[3] *= this->opacity;
		}

		return colors.Color;
	}
};