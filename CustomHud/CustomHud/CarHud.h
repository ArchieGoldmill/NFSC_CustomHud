#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "DALVehicleCarbon.h"
#include <map>
#include <vector>

using namespace std;

#define degToRad(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define radToDeg(angleRadians) ((angleRadians) * 180.0 / M_PI)

class Sprite
{
private:
	LPDIRECT3DDEVICE9 pDevice;
	LPD3DXSPRITE pSprite;
	LPDIRECT3DTEXTURE9 pTexture;
	bool isReleased;

public:
	D3DXIMAGE_INFO Info;

	Sprite(LPDIRECT3DDEVICE9 pDevice, string& path)
	{
		this->pDevice = pDevice;
		this->LoadTexture(path);
		this->GetTextureInfo(path);
		this->CreateSprite();
	}

	void Draw(RECT* rect, D3DCOLOR color)
	{
		this->pSprite->Begin(D3DXSPRITE_ALPHABLEND);

		this->pSprite->Draw(this->pTexture, rect, NULL, NULL, color);

		this->pSprite->End();
	}

	void SetTransform(D3DXMATRIX matrix)
	{
		this->pSprite->SetTransform(&matrix);
	}

	void Release()
	{
		if (!this->isReleased)
		{
			this->pSprite->Release();
			this->pTexture->Release();
			this->isReleased = true;
		}
	}

private:
	bool CreateSprite()
	{
		return FAILED(D3DXCreateSprite(this->pDevice, &pSprite));
	}

	bool LoadTexture(string& path)
	{
		return FAILED(D3DXCreateTextureFromFile(this->pDevice, path.c_str(), &this->pTexture));
	}

	bool GetTextureInfo(string& path)
	{
		return FAILED(D3DXGetImageInfoFromFile(path.c_str(), &this->Info));
	}
};

class HUD_Element
{
protected:
	LPDIRECT3DDEVICE9 pDevice;

public:
	virtual void Draw() = 0;

	virtual void Release() = 0;

	HUD_Element(LPDIRECT3DDEVICE9 pDevice)
	{
		this->pDevice = pDevice;
	}

protected:
	void Setup(Sprite* texture, D3DXVECTOR2 targetRes, D3DXVECTOR2 centerPercent, D3DXVECTOR2 positionOffset, RECT* rect, float rotation)
	{
		D3DVIEWPORT9 wndSize = GetWindowSize();
		float wscale = wndSize.Height / 1080.0;
		//float wscale = wndSize.Height / 600.0;

		targetRes.x *= wscale;
		targetRes.y *= wscale;

		positionOffset.x *= wscale;
		positionOffset.y *= wscale;

		D3DXVECTOR2 scale;
		scale.x = targetRes.x / texture->Info.Width;
		scale.y = targetRes.y / texture->Info.Height;

		D3DXVECTOR2 center;
		center.x = targetRes.x * centerPercent.x;
		center.y = targetRes.y * centerPercent.y;

		D3DXVECTOR2 offset;
		if (rect != NULL)
		{
			offset.x = abs(rect->right - rect->left) * wscale;
			offset.y = abs(rect->bottom - rect->top) * wscale;
		}
		else
		{
			offset.x = targetRes.x;
			offset.y = targetRes.y;
		}

		D3DXVECTOR2 position;
		position.x = wndSize.Width - offset.x - positionOffset.x;
		position.y = wndSize.Height - offset.y - positionOffset.y;

		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D(&matrix, NULL, NULL, &scale, &center, degToRad(rotation), &position);
		texture->SetTransform(matrix);
	}

private:
	D3DVIEWPORT9 GetWindowSize()
	{
		D3DVIEWPORT9 viewprot;
		this->pDevice->GetViewport(&viewprot);

		return viewprot;
	}
};

typedef float(FloatValueCallback)();
typedef int(IntValueCallback)();

struct HUD_Gauge_Callbacks
{
	FloatValueCallback* GetValue;
	FloatValueCallback* GetMaxValue;

	float ZeroAngle;
	float StepAngle;
};

class HUD_Gauge : HUD_Element
{
private:
	Sprite* numbers;
	Sprite* arrow;
	Sprite* background;
	float size;
	D3DXVECTOR2 position;
	HUD_Gauge_Callbacks callbacks;

public:
	HUD_Gauge(LPDIRECT3DDEVICE9 pDevice, float size, D3DXVECTOR2 position, Sprite* numbers, Sprite* arrow, Sprite* background, HUD_Gauge_Callbacks callbacks) : HUD_Element(pDevice)
	{
		this->pDevice = pDevice;

		this->numbers = numbers;
		this->arrow = arrow;
		this->background = background;

		this->size = size;
		this->position = position;
		this->callbacks = callbacks;
	}

	void Draw()
	{
		this->DrawNumbers();
		this->DrawArrow();
		this->DrawBackground();
	}

	void Release()
	{
		this->numbers->Release();
		this->arrow->Release();
		this->background->Release();
	}

	~HUD_Gauge()
	{
		this->Release();
	}

private:
	void DrawNumbers()
	{
		D3DXVECTOR2 center;
		center.x = 0;
		center.y = 0;

		float rpm = this->callbacks.GetValue();
		float redline = this->callbacks.GetMaxValue();

		D3DCOLOR color = D3DCOLOR_RGBA(150, 227, 255, 255);

		if (redline - rpm < 200)
		{
			color = D3DCOLOR_RGBA(255, 30, 30, 255);
		}

		this->Setup(this->numbers, { this->size,this->size }, center, this->position, NULL, 0);

		this->numbers->Draw(NULL, color);
	}

	void DrawBackground()
	{
		this->Setup(this->background, { this->size, this->size }, { 0, 0 }, this->position, NULL, 0);

		this->background->Draw(NULL, D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	void DrawArrow()
	{
		D3DXVECTOR2 targetRes;
		targetRes.x = this->size / 1.75;
		targetRes.y = this->size / 3.5;

		D3DXVECTOR2 center;
		center.x = 0.78;
		center.y = 0.5;

		D3DXVECTOR2 position;
		position.x = this->size / 2.69 + this->position.x;
		position.y = this->size / 2.77 + this->position.y;

		float rpm = this->callbacks.GetValue();

		float rotation = this->callbacks.ZeroAngle + this->callbacks.StepAngle * rpm / 1000.0;

		this->Setup(this->arrow, targetRes, center, position, NULL, rotation);

		this->arrow->Draw(NULL, D3DCOLOR_RGBA(255, 44, 44, 170));
	}
};

class HUD_Digit : HUD_Element
{
private:
	Sprite* digits;
	float size;
	D3DXVECTOR2 position;
	IntValueCallback* getNumber;
	D3DCOLOR color;

public:
	HUD_Digit(LPDIRECT3DDEVICE9 pDevice, float size, D3DXVECTOR2 position, Sprite* digits, IntValueCallback* getNumber, D3DCOLOR color) : HUD_Element(pDevice)
	{
		this->digits = digits;
		this->size = size;
		this->position = position;
		this->getNumber = getNumber;
		this->color = color;
	}

	void Draw()
	{
		D3DXVECTOR2 size;
		size.x = 12 * (this->size / 1.55);
		size.y = this->size;

		D3DXVECTOR2 center;
		center.x = 0;
		center.y = 0;

		int gear = this->getNumber();
		float numSize = this->digits->Info.Width / 12.0;

		RECT rect;
		rect.left = (gear - 1) * numSize + 1;
		rect.right = numSize * gear - 2;
		rect.top = 0;
		rect.bottom = this->digits->Info.Height;

		this->Setup(this->digits, size, center, this->position, &rect, 0);

		this->digits->Draw(&rect, color);
	}

	void Release()
	{
		this->digits->Release();
	}

	~HUD_Digit()
	{
		this->Release();
	}
};

class HUD_Tachometer : HUD_Element
{
private:
	HUD_Gauge* Gauge;
	HUD_Digit* Gear;

public:
	HUD_Tachometer(LPDIRECT3DDEVICE9 pDevice, float size, D3DXVECTOR2 position, Sprite* numbers, Sprite* arrow, Sprite* digits, Sprite* background) : HUD_Element(pDevice)
	{
		HUD_Gauge_Callbacks hudGaugeCallbacks;
		hudGaugeCallbacks.GetValue = GetRPM;
		hudGaugeCallbacks.GetMaxValue = GetRedline;
		hudGaugeCallbacks.ZeroAngle = -30;
		hudGaugeCallbacks.StepAngle = 24;
		this->Gauge = new HUD_Gauge(pDevice, size, position, numbers, arrow, background, hudGaugeCallbacks);

		this->Gear = new HUD_Digit(pDevice, 80, { 192, 104 }, digits, GetGear, D3DCOLOR_RGBA(255, 44, 44, 255));
	}

	void Draw()
	{
		if (this->Gauge != NULL)
		{
			this->Gauge->Draw();
		}

		if (this->Gear != NULL)
		{
			this->Gear->Draw();
		}
	}


	void Release()
	{
		if (this->Gauge != NULL)
		{
			this->Gauge->Release();
		}

		if (this->Gear != NULL)
		{
			this->Gear->Release();
		}
	}

	~HUD_Tachometer()
	{
		if (this->Gauge != NULL)
		{
			delete this->Gauge;
		}

		if (this->Gear != NULL)
		{
			delete this->Gear;
		}
	}

};

class HUD_Speedometer : HUD_Element
{
private:
	HUD_Digit* Speed[3];

public:
	HUD_Speedometer(LPDIRECT3DDEVICE9 pDevice, float size, float distance, D3DXVECTOR2 position, Sprite* digits) : HUD_Element(pDevice)
	{
		for (int i = 0; i < 3; i++)
		{
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

			D3DXVECTOR2 pos;
			pos.x = i * (size / 3.0f) + position.x + distance * i;
			pos.y = position.y;

			this->Speed[i] = new HUD_Digit(pDevice, size, pos, digits, cb, D3DCOLOR_RGBA(12, 26, 48, 255));
		}
	}

	void Draw()
	{
		for (int i = 0; i < 3; i++)
		{
			this->Speed[i]->Draw();
		}
	}

	void Release()
	{
		for (int i = 0; i < 3; i++)
		{
			this->Speed[i]->Release();
		}
	}

	~HUD_Speedometer()
	{
		for (int i = 0; i < 3; i++)
		{
			delete this->Speed[i];
		}
	}
};

class CarHud
{
private:
	IDirect3DDevice9* pDevice;
	hash<string> stringHasher;
	map<int, Sprite*> spritesMap;

	HUD_Tachometer* Tachometer;
	HUD_Speedometer* Speedometer;

public:
	static bool ShowHUD;

	CarHud(IDirect3DDevice9* pDevice, string path)
	{
		this->pDevice = pDevice;

		string temp = path;
		Sprite* tachNumbers = this->CreateSprite(temp.append("\\tach-numbers.dds"));

		temp = path;
		Sprite* tachArrow = this->CreateSprite(temp.append("\\tach-arrow.dds"));

		temp = path;
		Sprite* tachBackground = this->CreateSprite(temp.append("\\tach-background.dds"));

		temp = path;
		Sprite* digits = this->CreateSprite(temp.append("\\digits.dds"));

		this->Tachometer = new HUD_Tachometer(pDevice, 350, { 30, 0 }, tachNumbers, tachArrow, digits, tachBackground);

		this->Speedometer = new HUD_Speedometer(pDevice, 80, 10, { 155, 45 }, digits);
	}

	void Draw()
	{
		if (!IsHudVisible() || !IsPlayerControlling() || !CarHud::ShowHUD)
		{
			//return;
		}

		this->Tachometer->Draw();

		this->Speedometer->Draw();
	}

	~CarHud()
	{
		if (this->Tachometer != NULL)
		{
			delete this->Tachometer;
		}

		if (this->Speedometer != NULL)
		{
			delete this->Speedometer;
		}

		for (auto sprite : this->spritesMap)
		{
			delete sprite.second;
		}
	}

private:
	Sprite* CreateSprite(string& path)
	{
		int hash = stringHasher(path);
		for (auto i : this->spritesMap)
		{
			if (i.first == hash)
			{
				return i.second;
			}
		}

		Sprite* sprite = new Sprite(pDevice, path);
		this->spritesMap.insert({ hash, sprite });

		return sprite;
	}
};
bool CarHud::ShowHUD = false;