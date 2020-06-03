#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "DALVehicleCarbon.h"
#include <map>

using namespace std;

#define degToRad(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define radToDeg(angleRadians) ((angleRadians) * 180.0 / M_PI)

class HudTexture
{
public:
	ID3DXSprite* sprite;
	IDirect3DTexture9* texture;
	D3DXIMAGE_INFO info;
	D3DCOLOR color;
	RECT* Rect;

	HudTexture()
	{
		this->Rect = NULL;
	}

	void Draw()
	{
		this->sprite->Begin(D3DXSPRITE_ALPHABLEND);

		this->sprite->Draw(this->texture, this->Rect, NULL, NULL, this->color);

		this->sprite->End();
	}

	void Release()
	{
		this->sprite->Release();
		this->texture->Release();
		if (this->Rect != NULL)
		{
			delete this->Rect;
		}
	}
};

class CarHud
{
private:
	IDirect3DDevice9* pDevice;

	HudTexture tachNumbers;
	HudTexture tachArrow;
	HudTexture gear;

public:
	static bool ShowHUD;

	CarHud(IDirect3DDevice9* pDevice, string path)
	{
		this->pDevice = pDevice;

		string p = path;
		this->CreateHudTexture(&tachNumbers, p.append("\\tach-numbers.dds"));

		p = path;
		this->CreateHudTexture(&tachArrow, p.append("\\tach-arrow.dds"));

		p = path;
		this->CreateHudTexture(&gear, p.append("\\digits.dds"));
	}

	bool IsInit()
	{
		return this->pDevice != NULL;
	}

	void Draw()
	{
		if (!this->IsHudVisible() || !this->IsPlayerControlling() || !CarHud::ShowHUD)
		{
			return;
		}

		this->SetupTachNumbers();

		this->SetupTachArrow();

		this->SetupGear();
	}

	~CarHud()
	{
		this->tachArrow.Release();
		this->tachNumbers.Release();
		this->gear.Release();
	}

private:
	void SetupTachNumbers()
	{
		D3DXVECTOR2 size;
		size.x = 350;
		size.y = 350;

		D3DXVECTOR2 center;
		center.x = 0;
		center.y = 0;

		D3DXVECTOR2 position;
		position.x = 0;
		position.y = 0;

		float rpm;
		DALVehicle_GetRPM(NULL, &rpm, 0);

		float maxrpm;
		DALVehicle_GetRedLine(NULL, &maxrpm, 0);

		D3DCOLOR color = D3DCOLOR_RGBA(150, 227, 255, 255);

		if (maxrpm - rpm < 200)
		{
			color = D3DCOLOR_RGBA(255, 30, 30, 255);
		}

		this->Setup(&this->tachNumbers, size, color, center, position, 0);

		this->tachNumbers.Draw();
	}

	void SetupTachArrow()
	{
		D3DXVECTOR2 targetRes;
		targetRes.x = 200;
		targetRes.y = 100;

		D3DXVECTOR2 center;
		center.x = 0.78;
		center.y = 0.5;

		D3DXVECTOR2 position;
		position.x = 130;
		position.y = 126;

		float rpm;
		DALVehicle_GetRPM(NULL, &rpm, 0);

		float rotation = -30 + 24 * rpm / 1000.0;

		this->Setup(&this->tachArrow, targetRes, D3DCOLOR_RGBA(255, 44, 44, 170), center, position, rotation);

		this->tachArrow.Draw();
	}

	void SetupGear()
	{
		int gear;
		DALVehicle_GetGear(NULL, &gear, 0);

		if (gear == 0)
		{
			gear = 12;
		}

		if (gear == 1)
		{
			gear = 11;
		}

		if (gear > 12 || gear < 0)
		{
			gear = 0;
		}

		D3DXVECTOR2 size;
		size.x = 12 * 45;
		size.y = 68;

		D3DXVECTOR2 center;
		center.x = 0;
		center.y = 0;

		D3DXVECTOR2 position;
		position.x = -348;
		position.y = 90;

		this->Setup(&this->gear, size, D3DCOLOR_RGBA(255, 44, 44, 255), center, position, 0);

		if (this->gear.Rect == NULL)
		{
			this->gear.Rect = new RECT();

		}

		int numSize = 43;

		this->gear.Rect->left = (gear - 1) * numSize + 1;
		this->gear.Rect->right = numSize * gear - 2;
		this->gear.Rect->top = 0;
		this->gear.Rect->bottom = this->gear.info.Height;

		this->gear.Draw();
	}

	void Setup(HudTexture * texture, D3DXVECTOR2 targetRes, D3DCOLOR color, D3DXVECTOR2 centerPercent, D3DXVECTOR2 positionOffset, float rotation)
	{
		texture->color = color;

		D3DVIEWPORT9 wndSize = GetWindowSize();
		float wscale = wndSize.Height / 1080.0;

		targetRes.x *= wscale;
		targetRes.y *= wscale;

		positionOffset.x *= wscale;
		positionOffset.y *= wscale;

		D3DXVECTOR2 scale;
		scale.x = targetRes.x / texture->info.Width;
		scale.y = targetRes.y / texture->info.Height;

		D3DXVECTOR2 center;
		center.x = targetRes.x * centerPercent.x;
		center.y = targetRes.y * centerPercent.y;

		D3DXVECTOR2 generalOffset;
		generalOffset.x = 30;
		generalOffset.y = 0;

		D3DXVECTOR2 position;
		position.x = wndSize.Width - targetRes.x - positionOffset.x - generalOffset.x;
		position.y = wndSize.Height - targetRes.y - positionOffset.y - generalOffset.y;

		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D(&matrix, NULL, NULL, &scale, &center, degToRad(rotation), &position);
		texture->sprite->SetTransform(&matrix);
	}

	void CreateHudTexture(HudTexture * hudTexture, string path)
	{
		// TODO Add texture map
		this->LoadTexture(&hudTexture->texture, path);
		this->GetTextureInfo(&hudTexture->info, path);
		this->CreateSprite(&hudTexture->sprite);
	}

	bool LoadTexture(IDirect3DTexture9 * *pTexture, string path)
	{
		return FAILED(D3DXCreateTextureFromFile(this->pDevice, path.c_str(), pTexture));
	}

	bool  GetTextureInfo(D3DXIMAGE_INFO * info, string path)
	{
		return FAILED(D3DXGetImageInfoFromFile(path.c_str(), info));
	}

	bool CreateSprite(ID3DXSprite * *pSprite)
	{
		return FAILED(D3DXCreateSprite(this->pDevice, pSprite));
	}

	D3DVIEWPORT9 GetWindowSize()
	{
		D3DVIEWPORT9 viewprot;
		this->pDevice->GetViewport(&viewprot);

		return viewprot;
	}

	bool IsHudVisible()
	{
		int res;
		DALVehicle_GetIsHudVisible(NULL, &res, 0);
		return res != 0;
	}

	bool IsPlayerControlling()
	{
		int res;
		DALVehicle_GetIsPlayerControlling(NULL, &res, 0);
		return res != 0;
	}
};

bool CarHud::ShowHUD = false;