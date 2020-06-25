#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "ini/IniReader.h"
#include <string.h>

using namespace std;

class HUD_Params;
namespace Global
{
	extern HUD_Params HUDParams;
	string GetHudPath();
	void Init();
};

typedef float(FloatValueCallback)();
typedef int(IntValueCallback)();
typedef bool(BoolValueCallback)();

class HUD_Element_Params
{
public:
	bool Enabled;
	float Size;
	D3DXVECTOR2 Position;
	D3DCOLOR Color;

	void Init(CIniReader& ini, char* category)
	{
		this->Enabled = ini.ReadInteger(category, (char*)"Enabled", 0);
		if (this->Enabled)
		{
			this->Size = ini.ReadFloat(category, (char*)"Size", 0.0f);

			this->Position.x = ini.ReadFloat(category, (char*)"PositionX", 0.0f);
			this->Position.y = ini.ReadFloat(category, (char*)"PositionY", 0.0f);
			this->Color = ini.ReadInteger(category, (char*)"Color", 0);
		}
	}

	string ReadString(CIniReader& ini, char* category, const char* field)
	{
		string res = "scripts\\";
		string tex = ini.ReadString(category, (char*)field, "");
		if (!tex.empty())
		{
			return res.append(Global::GetHudPath()).append(tex);
		}
		else
		{
			return tex;
		}
	}
};

class HUD_Digit_Params : public HUD_Element_Params
{
public:
	IntValueCallback* GetNumber;
	string TextureDigits;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Element_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->TextureDigits = this->ReadString(ini, category, "TextureDigits");
		}
	}
};

class HUD_Units_Params : public HUD_Element_Params
{
public:
	string TextureUnits;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Element_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->TextureUnits = this->ReadString(ini, category, "TextureUnits");
		}
	}
};

class HUD_Filled_Params : public HUD_Element_Params
{
public:
	string TextureFilled;
	string TextureBackground;
	float Value;
	bool Direction;
	FloatValueCallback* GetValue;
	D3DCOLOR BackgroundColor;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Element_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->TextureFilled = this->ReadString(ini, category, "TextureFilled");
			this->TextureBackground = this->ReadString(ini, category, "TextureBackground");
			this->Value = ini.ReadFloat(category, (char*)"Value", 0.0f);
			this->Direction = ini.ReadInteger(category, (char*)"Direction", 0);
			this->BackgroundColor = ini.ReadInteger(category, (char*)"BackgroundColor", 0);
		}
	}
};

class HUD_Speedometer_Params : public HUD_Digit_Params
{
public:

	float Distance;
	HUD_Units_Params Units;
	bool ShowAllDigits;

	void Init(CIniReader& ini)
	{
		char* category = (char*)"SPEEDOMETER";
		HUD_Digit_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->Distance = ini.ReadFloat(category, (char*)"Distance", 0.0f);
			this->ShowAllDigits = ini.ReadInteger(category, (char*)"ShowAllDigits", 1);

			this->Units.Init(ini, (char*)"UNITS");
		}
	}
};

class HUD_Gauge_Params : public HUD_Element_Params
{
public:
	float ArrowMinAngle;
	float ArrowMaxAngle;
	float Value;
	float NumbersMaxThreshold;

	D3DCOLOR ArrowColor;
	D3DCOLOR ArrowMaskedColor;
	D3DCOLOR BackgroundColor;
	D3DCOLOR BackgroundMaskedColor;
	D3DCOLOR NumbersColor;
	D3DCOLOR NumbersMaxColor;
	D3DCOLOR PerfectZoneColor;

	D3DCOLOR BackgroundMaskedColor1;
	D3DCOLOR BackgroundMaskedColor2;

	D3DCOLOR ArrowMaskedColor1;
	D3DCOLOR ArrowMaskedColor2;

	string TextureBackground;
	string TextureNumbers;
	string TextureArrow;
	string TextureBackgroundMasked;
	string TextureArrowMasked;
	float BackgroundMaskedSize;
	float ArrowMaskedSize;

	FloatValueCallback* GetArrowValue;
	FloatValueCallback* GetMaxValue;

	FloatValueCallback* GetMaskValue1;
	FloatValueCallback* GetMaskValue2;

	FloatValueCallback* GetArrowMaskValue1;
	FloatValueCallback* GetArrowMaskValue2;

	BoolValueCallback* IsInperfectZone;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Element_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->BackgroundMaskedSize = ini.ReadFloat(category, (char*)"BackgroundMaskedSize", 128.0f);
			this->ArrowMaskedSize = ini.ReadFloat(category, (char*)"ArrowMaskedSize", 128.0f);

			this->ArrowMinAngle = ini.ReadFloat(category, (char*)"ArrowMinAngle", 0.0f);
			this->ArrowMaxAngle = ini.ReadFloat(category, (char*)"ArrowMaxAngle", 0.0f);
			this->Value = ini.ReadFloat(category, (char*)"Value", 0.0f);
			this->NumbersMaxThreshold = ini.ReadFloat(category, (char*)"NumbersMaxThreshold", -1.0f);

			this->ArrowColor = ini.ReadInteger(category, (char*)"ArrowColor", 0);
			this->BackgroundColor = ini.ReadInteger(category, (char*)"BackgroundColor", 0);
			this->BackgroundMaskedColor = ini.ReadInteger(category, (char*)"BackgroundMaskedColor", 0);
			this->NumbersColor = ini.ReadInteger(category, (char*)"NumbersColor", 0);
			this->NumbersMaxColor = ini.ReadInteger(category, (char*)"NumbersMaxColor", 0);
			this->BackgroundMaskedColor1 = ini.ReadInteger(category, (char*)"BackgroundMaskedColor1", 0);
			this->BackgroundMaskedColor2 = ini.ReadInteger(category, (char*)"BackgroundMaskedColor2", 0);
			this->PerfectZoneColor = ini.ReadInteger(category, (char*)"PerfectZoneColor", 0);
			this->ArrowMaskedColor = ini.ReadInteger(category, (char*)"ArrowMaskedColor", 0);
			this->ArrowMaskedColor1 = ini.ReadInteger(category, (char*)"ArrowMaskedColor1", 0);
			this->ArrowMaskedColor2 = ini.ReadInteger(category, (char*)"ArrowMaskedColor2", 0);

			this->TextureBackground = this->ReadString(ini, category, "TextureBackground");
			this->TextureNumbers = this->ReadString(ini, category, (char*)"TextureNumbers");
			this->TextureArrow = this->ReadString(ini, category, (char*)"TextureArrow");
			this->TextureBackgroundMasked = this->ReadString(ini, category, (char*)"TextureBackgroundMasked");
			this->TextureArrowMasked = this->ReadString(ini, category, (char*)"TextureArrowMasked");
		}
	}
};

class HUD_Tachometer_Params
{
public:
	HUD_Digit_Params GearParams;
	HUD_Gauge_Params GaugeParams;

	void Init(CIniReader& ini)
	{
		this->GearParams.Init(ini, (char*)"GEAR");
		this->GaugeParams.Init(ini, (char*)"TACHOMETER");
	}
};

class HUD_Params
{
public:
	HUD_Tachometer_Params Tachometer;
	HUD_Gauge_Params Nos;
	HUD_Filled_Params NosFilled;
	HUD_Gauge_Params Boost;
	HUD_Gauge_Params SpeedBreak;
	HUD_Speedometer_Params Speedometer;
	float Scale;
	D3DXVECTOR2 Offset;

	void Init(CIniReader& ini)
	{
		this->Tachometer.Init(ini);
		this->Speedometer.Init(ini);
		this->Nos.Init(ini, (char*)"NOS");
		this->NosFilled.Init(ini, (char*)"NOS_FILLED");
		this->Boost.Init(ini, (char*)"BOOST");
		this->SpeedBreak.Init(ini, (char*)"SPEEDBREAK");
	}
};