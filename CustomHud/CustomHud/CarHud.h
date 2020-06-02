#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "DALVehicleCarbon.h"

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
	D3DXVECTOR3 position;

	HudTexture()
	{
		this->position.x = 0;
		this->position.y = 0;
		this->position.z = 0;
	}

	void Draw()
	{
		this->sprite->Begin(D3DXSPRITE_ALPHABLEND);

		this->sprite->Draw(this->texture, NULL, NULL, &this->position, this->color);

		this->sprite->End();
	}

	void Release()
	{
		this->sprite->Release();
		this->texture->Release();
	}
};

class CarHud
{
private:
	IDirect3DDevice9* pDevice;

	HudTexture tachNumbers;
	HudTexture tachArrow;

public:
	CarHud(IDirect3DDevice9* pDevice, string path)
	{
		this->pDevice = pDevice;

		string p = path;
		this->CreateHudTexture(&tachNumbers, p.append("\\tach-numbers.dds"));
		p = path;
		this->CreateHudTexture(&tachArrow, p.append("\\tach-arrow.dds"));
	}

	bool IsInit()
	{
		return this->pDevice != NULL;
	}

	void Draw()
	{
		if (!this->IsHudVisible() || !this->IsPlayerControlling())
		{
			//return;
		}

		this->SetupTachNumbers();
		this->tachNumbers.Draw();

		this->SetupTachArrow();
		this->tachArrow.Draw();
	}

	~CarHud()
	{
		this->tachArrow.Release();
		this->tachNumbers.Release();
	}

private:
	void SetupTachNumbers()
	{
		D3DXVECTOR2 targetRes;
		targetRes.x = 356.0;
		targetRes.y = 356.0;

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
			color = D3DCOLOR_RGBA(255, 0, 0, 255);
		}

		this->Setup(&this->tachNumbers, targetRes, color, center, position, 0);
	}

	void SetupTachArrow()
	{
		D3DXVECTOR2 targetRes;
		targetRes.x = 100;
		targetRes.y = 10;

		D3DXVECTOR2 center;
		center.x = 1.15;
		center.y = 0.5;

		D3DXVECTOR2 position;
		position.x = 193;
		position.y = 179;

		float rpm;
		DALVehicle_GetRPM(NULL, &rpm, 0);

		float rotation = -30 + 24 * rpm / 1000.0;

		this->Setup(&this->tachArrow, targetRes, D3DCOLOR_RGBA(200, 0, 0, 255), center, position, rotation);
	}

	void Setup(HudTexture* texture, D3DXVECTOR2 targetRes, D3DCOLOR color, D3DXVECTOR2 centerPercent, D3DXVECTOR2 positionOffset, float rotation)
	{
		texture->color = color;

		D3DXVECTOR2 scale;
		scale.x = targetRes.x / texture->info.Width;
		scale.y = targetRes.y / texture->info.Height;

		D3DXVECTOR2 center;
		center.x = targetRes.x * centerPercent.x;
		center.y = targetRes.y * centerPercent.y;

		RECT wndSize = GetWindowSize();
		D3DXVECTOR2 position;
		position.x = wndSize.right - targetRes.x - positionOffset.x;
		position.y = wndSize.bottom - targetRes.y - positionOffset.y;

		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D(&matrix, NULL, NULL, &scale, &center, degToRad(rotation), &position);
		texture->sprite->SetTransform(&matrix);
	}

	void CreateHudTexture(HudTexture* hudTexture, string path)
	{
		this->LoadTexture(&hudTexture->texture, path);
		this->GetTextureInfo(&hudTexture->info, path);
		this->CreateSprite(&hudTexture->sprite);
	}

	bool LoadTexture(IDirect3DTexture9** pTexture, string path)
	{
		return FAILED(D3DXCreateTextureFromFile(this->pDevice, path.c_str(), pTexture));
	}

	bool  GetTextureInfo(D3DXIMAGE_INFO* info, string path)
	{
		return FAILED(D3DXGetImageInfoFromFile(path.c_str(), info));
	}

	bool CreateSprite(ID3DXSprite** pSprite)
	{
		return FAILED(D3DXCreateSprite(this->pDevice, pSprite));
	}

	RECT GetWindowSize()
	{
		HWND wnd = GetActiveWindow();
		RECT result;
		GetWindowRect(wnd, &result);

		return result;
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