#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

using namespace std;

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
