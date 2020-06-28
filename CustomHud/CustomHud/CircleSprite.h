#pragma once

#include "Sprite.h"
#include <math.h>
#include <windows.h>

#include <chrono>

#define isCCW(a, b, c) (((a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x)) > 0.0f)
#define isInRegion(O, A, B, C) (isCCW(O, A, P) && !isCCW(O, B, P))

class CircleSprite : public Sprite
{
private:
	D3DXVECTOR2 center;
	float angle1;
	float angle2;
	int color1;
	int color2;

public:
	CircleSprite(LPDIRECT3DDEVICE9 pDevice, string& path, D3DXVECTOR2* maskSize) : Sprite(pDevice, path, maskSize)
	{

	}

	void SetupMask(D3DXVECTOR2 center, float angle1, float angle2, int color1, int color2)
	{
		this->center = center;
		this->angle1 = angle1;
		this->angle2 = angle2;
		this->color1 = color1;
		this->color2 = color2;
	}

	virtual void DrawMask(int* mask)
	{
		auto start = chrono::steady_clock::now();

		D3DXVECTOR2 O = { this->maskSize.x * this->center.x, this->maskSize.y * this->center.y };
		bool invert = abs(angle2 - angle1) > 180.0f;

		D3DXVECTOR2 A, B;
		if (!invert)
		{
			A = this->GetLineEnd(O, this->angle1);
			B = this->GetLineEnd(O, this->angle2);
		}
		else
		{
			A = this->GetLineEnd(O, this->angle2);
			B = this->GetLineEnd(O, this->angle1);
		}

		int h = this->maskSize.y;
		int w = this->maskSize.x;

		D3DXVECTOR2 P;
		int j;
		int color;
		for (int i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				color = this->color1;
				P.x = j;
				P.y = i;
				bool inReg = isInRegion(O, A, B, P);
				if (invert)
				{
					inReg = !inReg;
				}

				if (inReg)
				{
					color = this->color2;
				}

				mask[i * w + j] = color;
			}
		}

		auto now = chrono::steady_clock::now();

		int a = chrono::duration_cast<std::chrono::microseconds>(now - start).count();

		int b = a;
	}

private:

	D3DXVECTOR2 GetLineEnd(D3DXVECTOR2 & a, float angle)
	{
		angle -= 180;

		D3DXVECTOR2 res;
		float rad = degToRad(angle);
		float len = max(this->maskSize.x, this->maskSize.y);

		res.x = a.x + cos(rad) * len;
		res.y = a.y + sin(rad) * len;

		return res;
	}
};