#pragma once
#include <string>
#include "Sprite.h"
#include "Globals.h"
#include "GameApi.h"

#define degToRad(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define radToDeg(angleRadians) ((angleRadians) * 180.0 / M_PI)

class HUD_Element
{
protected:
	LPDIRECT3DDEVICE9 pDevice;
	bool isReleased;

public:
	virtual void Release() = 0;

	HUD_Element(LPDIRECT3DDEVICE9 pDevice)
	{
		this->isReleased = false;
		this->pDevice = pDevice;
	}

protected:
	void Setup(Sprite* sprite, D3DXVECTOR2 targetRes, D3DXVECTOR2 centerPercent, D3DXVECTOR2 positionOffset, D3DXVECTOR2* rect, float rotation)
	{
		D3DVIEWPORT9 wndSize = GetWindowSize();
		float wscale = wndSize.Height / 1080.0f * Global::HUDParams.Scale;

		D3DXVECTOR2 offset = rect != NULL ? *rect : targetRes;

		D3DXVECTOR2 position;
		position.x = wndSize.Width - (offset.x + positionOffset.x + Global::HUDParams.Offset.x) * wscale;
		position.y = wndSize.Height - (offset.y + positionOffset.y + Global::HUDParams.Offset.y) * wscale;
		this->ApplyShake(position, wscale);

		targetRes.x *= wscale;
		targetRes.y *= wscale;

		D3DXVECTOR2 scale;
		scale.x = targetRes.x / sprite->Info.Width;
		scale.y = targetRes.y / sprite->Info.Height;

		D3DXVECTOR2 center;
		center.x = targetRes.x * centerPercent.x;
		center.y = targetRes.y * centerPercent.y;

		D3DXMATRIX matrix;
		D3DXMatrixTransformation2D(&matrix, NULL, NULL, &scale, &center, degToRad(rotation), &position);
		sprite->SetTransform(matrix);
	}

private:
	D3DVIEWPORT9 GetWindowSize()
	{
		D3DVIEWPORT9 viewprot;
		this->pDevice->GetViewport(&viewprot);
		return viewprot;
	}

	void ApplyShake(D3DXVECTOR2& position, float scale)
	{
		if (Global::HUDParams.ShakeAmount)
		{
			float shake = Game::GetShake() * scale;
			if (shake)
			{
				shake *= Global::HUDParams.ShakeAmount;
				position.x += shake;
				position.y += shake;
			}
		}
	}
};