#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "ini/IniReader.h"
#include <string.h>

class HUD_Params;
namespace Global
{
	extern HUD_Params HUDParams;
	std::string GetHudPath();
	void Init();
	bool CarHasHud(std::string name);
	bool ShowVanilla();
	bool IsFileExist(std::string path);
	extern std::string CurrentCar;
	extern bool CarHasHUD;
	extern float DeltaTime;
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

	std::string ReadTexturePath(CIniReader& ini, char* category, const char* field)
	{
		char* str = ini.ReadString(category, (char*)field, "");
		std::string tex(str);
		delete str;

		if (!tex.empty())
		{
			return Global::GetHudPath().append(tex);
		}
		else
		{
			return tex;
		}
	}
};

class HUD_ShiftIcon_Params : public HUD_Element_Params
{
public:
	std::string IconTexture;
	int BlinkMode;
	bool IconTexturBlendMode;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Element_Params::Init(ini, category);
		this->IconTexture = this->ReadTexturePath(ini, category, "IconTexture");
		this->IconTexturBlendMode = ini.ReadInteger(category, (char*)"IconTexturBlendMode", 0);
		this->BlinkMode = ini.ReadInteger(category, (char*)"BlinkMode", 0);
	}
};

class HUD_Digit_Params : public HUD_Element_Params
{
public:
	IntValueCallback* GetNumber;
	std::string DigitsTexture;
	bool DigitsTextureBlendMode;
	D3DCOLOR BackgroundColor;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Element_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->DigitsTexture = this->ReadTexturePath(ini, category, "DigitsTexture");
			this->DigitsTextureBlendMode = ini.ReadInteger(category, (char*)"DigitsTextureBlendMode", 0);
			this->BackgroundColor = ini.ReadInteger(category, (char*)"BackgroundColor", 0);
		}
	}
};

class HUD_Units_Params : public HUD_Element_Params
{
public:
	std::string UnitsTexture;
	bool UnitsTextureBlendMode;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Element_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->UnitsTexture = this->ReadTexturePath(ini, category, "UnitsTexture");
			this->UnitsTextureBlendMode = ini.ReadInteger(category, (char*)"UnitsTextureBlendMode", 0);
		}
	}
};

class HUD_Liniar_Params : public HUD_Element_Params
{
public:
	std::string FilledTexture;
	D3DCOLOR FilledColor;
	bool FilledTextureBlendMode;

	D3DCOLOR BackgroundColor;
	std::string BackgroundTexture;
	bool BackgroundTextureBlendMode;

	float Value;
	bool Direction;
	FloatValueCallback* GetValue;
	BoolValueCallback* IsInstalled;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Element_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->Value = ini.ReadFloat(category, (char*)"Value", 0.0f);
			this->Direction = ini.ReadInteger(category, (char*)"Direction", 0);

			this->FilledTexture = this->ReadTexturePath(ini, category, "FilledTexture");
			this->FilledColor = ini.ReadInteger(category, (char*)"FilledColor", 0);
			this->FilledTextureBlendMode = ini.ReadInteger(category, (char*)"FilledTextureBlendMode", 0);

			this->BackgroundColor = ini.ReadInteger(category, (char*)"BackgroundColor", 0);
			this->BackgroundTexture = this->ReadTexturePath(ini, category, "BackgroundTexture");
			this->BackgroundTextureBlendMode = ini.ReadInteger(category, (char*)"BackgroundTextureBlendMode", 0);
		}
	}
};

class HUD_Digital_Params : public HUD_Digit_Params
{
public:
	std::string DigitsTexture1;
	std::string DigitsTexture2;

	float Distance;
	int DigitsMode;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Digit_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->Distance = ini.ReadFloat(category, (char*)"Distance", 0.0f);
			this->DigitsMode = ini.ReadInteger(category, (char*)"DigitsMode", 0);

			this->DigitsTexture1 = this->ReadTexturePath(ini, category, "DigitsTexture1");
			this->DigitsTexture2 = this->ReadTexturePath(ini, category, "DigitsTexture2");
		}
	}
};

class HUD_Gauge_Params : public HUD_Element_Params
{
public:
	float Value;
	bool CanBeNegative;

	float ArrowMinAngle;
	float ArrowMaxAngle;
	float ArrowMaskedSize;
	float ArrowScale;
	float ArrowCenterOffset;
	D3DCOLOR ArrowColor;
	D3DCOLOR ArrowMaskedColor;
	D3DCOLOR ArrowMaskedMaxColor;
	float ArrowMaskedMaxThreshold;
	D3DCOLOR ArrowMaskedColor1;
	D3DCOLOR ArrowMaskedColor2;
	std::string ArrowTexture;
	std::string ArrowMaskedTexture;
	D3DCOLOR ArrowPerfectZoneColor;
	bool ArrowTextureBlendMode;
	bool ArrowMaskedTextureBlendMode;
	float ArrowMaskedMinOffset;

	D3DCOLOR BackgroundColor;
	std::string BackgroundTexture;
	bool BackgroundTextureBlendMode;

	float NumbersMaskedSize;
	float NumbersMaxThreshold;
	D3DCOLOR NumbersColor;
	D3DCOLOR NumbersMaxColor;
	std::string NumbersTexture;
	bool NumbersTextureBlendMode;
	bool NumbersMaskedTextureBlendMode;
	D3DCOLOR NumbersMaskedColor;
	D3DCOLOR NumbersMaskedColor1;
	D3DCOLOR NumbersMaskedColor2;
	float NumbersMaskedMaxOffset;
	std::string NumbersMaskedTexture;

	FloatValueCallback* GetArrowValue;
	FloatValueCallback* GetMaxValue;

	FloatValueCallback* GetMaskValue1;
	FloatValueCallback* GetMaskValue2;

	FloatValueCallback* GetArrowMaskValue1;
	FloatValueCallback* GetArrowMaskValue2;

	BoolValueCallback* IsInstalled;
	BoolValueCallback* IsInperfectZone;

	void Init(CIniReader& ini, char* category)
	{
		HUD_Element_Params::Init(ini, category);
		if (this->Enabled)
		{
			this->Value = ini.ReadFloat(category, (char*)"Value", 0.0f);

			this->ArrowColor = ini.ReadInteger(category, (char*)"ArrowColor", 0);
			this->ArrowMaskedSize = ini.ReadFloat(category, (char*)"ArrowMaskedSize", 128.0f);
			this->ArrowMinAngle = ini.ReadFloat(category, (char*)"ArrowMinAngle", 0.0f);
			this->ArrowMaxAngle = ini.ReadFloat(category, (char*)"ArrowMaxAngle", 0.0f);
			this->ArrowScale = ini.ReadFloat(category, (char*)"ArrowScale", 1.0f);
			this->ArrowCenterOffset = ini.ReadFloat(category, (char*)"ArrowCenterOffset", 0.22f);
			this->ArrowMaskedMinOffset = ini.ReadFloat(category, (char*)"ArrowMaskedMinOffset", 0.0f);
			this->ArrowMaskedMaxThreshold = ini.ReadFloat(category, (char*)"ArrowMaskedMaxThreshold", 0.0f);
			this->ArrowMaskedColor = ini.ReadInteger(category, (char*)"ArrowMaskedColor", 0);
			this->ArrowMaskedMaxColor = ini.ReadInteger(category, (char*)"ArrowMaskedMaxColor", 0);
			this->ArrowMaskedColor1 = ini.ReadInteger(category, (char*)"ArrowMaskedColor1", 0);
			this->ArrowMaskedColor2 = ini.ReadInteger(category, (char*)"ArrowMaskedColor2", 0);
			this->ArrowPerfectZoneColor = ini.ReadInteger(category, (char*)"ArrowPerfectZoneColor", 0);
			this->ArrowTexture = this->ReadTexturePath(ini, category, (char*)"ArrowTexture");
			this->ArrowMaskedTexture = this->ReadTexturePath(ini, category, (char*)"ArrowMaskedTexture");
			this->ArrowTextureBlendMode = ini.ReadInteger(category, (char*)"ArrowTextureBlendMode", 0);
			this->ArrowMaskedTextureBlendMode = ini.ReadInteger(category, (char*)"ArrowMaskedTextureBlendMode", 0);

			this->NumbersMaxThreshold = ini.ReadFloat(category, (char*)"NumbersMaxThreshold", -1.0f);
			this->NumbersTexture = this->ReadTexturePath(ini, category, (char*)"NumbersTexture");
			this->NumbersColor = ini.ReadInteger(category, (char*)"NumbersColor", 0);
			this->NumbersMaxColor = ini.ReadInteger(category, (char*)"NumbersMaxColor", 0);
			this->NumbersTextureBlendMode = ini.ReadInteger(category, (char*)"NumbersTextureBlendMode", 0);
			this->NumbersMaskedTextureBlendMode = ini.ReadInteger(category, (char*)"NumbersMaskedTextureBlendMode", 0);
			this->NumbersMaskedColor = ini.ReadInteger(category, (char*)"NumbersMaskedColor", 0);
			this->NumbersMaskedColor1 = ini.ReadInteger(category, (char*)"NumbersMaskedColor1", 0);
			this->NumbersMaskedColor2 = ini.ReadInteger(category, (char*)"NumbersMaskedColor2", 0);
			this->NumbersMaskedTexture = this->ReadTexturePath(ini, category, (char*)"NumbersMaskedTexture");
			this->NumbersMaskedSize = ini.ReadFloat(category, (char*)"NumbersMaskedSize", 128.0f);
			this->NumbersMaskedMaxOffset = ini.ReadFloat(category, (char*)"NumbersMaskedMaxOffset", 0.0f);

			this->BackgroundColor = ini.ReadInteger(category, (char*)"BackgroundColor", 0);
			this->BackgroundTexture = this->ReadTexturePath(ini, category, "BackgroundTexture");
			this->BackgroundTextureBlendMode = ini.ReadInteger(category, (char*)"BackgroundTextureBlendMode", 0);

			this->CanBeNegative = ini.ReadInteger(category, (char*)"CanBeNegative", 1);
		}
	}
};

class HUD_Params
{
public:
	HUD_Gauge_Params TachometerGauge;
	HUD_Gauge_Params SpeedometerGauge;
	HUD_Gauge_Params NosGauge;
	HUD_Gauge_Params BoostGauge;
	HUD_Gauge_Params SpeedBreakGauge;

	HUD_Liniar_Params TachometerLiniar;
	HUD_Liniar_Params NosLiniar;
	HUD_Liniar_Params SpeedBreakLiniar;

	HUD_Digital_Params SpeedometerDigital;
	HUD_Digital_Params TachometerDigital;
	HUD_Units_Params Units;
	HUD_Digit_Params Gear;
	HUD_ShiftIcon_Params ShiftIcon;

	float Scale;
	D3DXVECTOR2 Offset;
	bool ReplaceDragHud;
	int HotReloadKey;
	bool ShowDebugInfo;
	int CustomCarHUDs;
	float ShakeAmount;

	void Init(CIniReader& ini)
	{
		this->TachometerGauge.Init(ini, (char*)"TACHOMETER_GAUGE");
		this->TachometerDigital.Init(ini, (char*)"TACHOMETER_DIGITAL");
		this->TachometerLiniar.Init(ini, (char*)"TACHOMETER_LINIAR");
		this->SpeedometerGauge.Init(ini, (char*)"SPEEDOMETER_GAUGE");
		this->SpeedometerDigital.Init(ini, (char*)"SPEEDOMETER_DIGITAL");
		this->NosGauge.Init(ini, (char*)"NOS_GAUGE");
		this->NosLiniar.Init(ini, (char*)"NOS_LINIAR");
		this->BoostGauge.Init(ini, (char*)"BOOST_GAUGE");
		this->SpeedBreakGauge.Init(ini, (char*)"SPEEDBREAK_GAUGE");
		this->SpeedBreakLiniar.Init(ini, (char*)"SPEEDBREAK_LINIAR");
		this->Units.Init(ini, (char*)"UNITS");
		this->Gear.Init(ini, (char*)"GEAR");
		this->ShiftIcon.Init(ini, (char*)"SHIFT_ICON");
	}
};