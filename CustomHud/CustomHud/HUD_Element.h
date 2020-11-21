#pragma once
#include <string>
#include "Sprite.h"
#include "Globals.h"

#define degToRad(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define radToDeg(angleRadians) ((angleRadians) * 180.0 / M_PI)

class HUD_Element
{
protected:
	LPDIRECT3DDEVICE9 pDevice;
	bool isReleased;

public:
	virtual void Draw() = 0;

	virtual void Release() = 0;

	HUD_Element(LPDIRECT3DDEVICE9 pDevice)
	{
		this->isReleased = false;
		this->pDevice = pDevice;
	}

protected:
	void Setup(Sprite* texture, D3DXVECTOR2 targetRes, D3DXVECTOR2 centerPercent, D3DXVECTOR2 positionOffset, D3DXVECTOR2* rect, float rotation)
	{
		D3DVIEWPORT9 wndSize = GetWindowSize();
		float wscale = wndSize.Height / 1080.0f * Global::HUDParams.Scale;

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
			offset.x = rect->x * wscale;
			offset.y = rect->y * wscale;
		}
		else
		{
			offset.x = targetRes.x;
			offset.y = targetRes.y;
		}

		D3DXVECTOR2 position;
		position.x = wndSize.Width - offset.x - positionOffset.x - Global::HUDParams.Offset.x;
		position.y = wndSize.Height - offset.y - positionOffset.y - Global::HUDParams.Offset.y;

		position.x += Global::GetRand().x;
		position.y += Global::GetRand().y;

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