#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Sprite
{
private:
	static hash<string> stringHasher;
	static map<int, LPDIRECT3DTEXTURE9> texturesMap;

	LPD3DXSPRITE pSprite;
	LPDIRECT3DTEXTURE9 pTexture;

	bool isReleased;
	bool BlendMode;

	string path;

protected:
	LPDIRECT3DDEVICE9 pDevice;
	D3DXVECTOR2 maskSize;
	LPDIRECT3DTEXTURE9 pMaskTexture;
	bool useMask;

public:
	D3DXIMAGE_INFO InfoOriginal;
	D3DSURFACE_DESC Info;

	Sprite(LPDIRECT3DDEVICE9 pDevice, string& path, bool BlendMode, D3DXVECTOR2 maskSize)
	{
		this->path = path;
		this->isReleased = false;
		this->BlendMode = BlendMode;
		this->pDevice = pDevice;
		this->LoadTexture(path);
		if (!this->pTexture)
		{
			throw "Unable to load texture from: \n" + path;
		}

		this->GetTextureInfo();
		this->CreateSprite();
	}

	void Draw(RECT* rect, D3DCOLOR color)
	{
		this->pSprite->Begin(D3DXSPRITE_ALPHABLEND);

		if (this->BlendMode)
		{
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
		}
		else
		{
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		if (this->useMask)
		{
			D3DLOCKED_RECT lockRectMask;
			this->pMaskTexture->LockRect(0, &lockRectMask, NULL, D3DLOCK_DISCARD);

			this->DrawMask((int*)lockRectMask.pBits);

			this->pMaskTexture->UnlockRect(0);

			int pTextureStage = 0;
			int pMaskTextureStage = 1;

			this->pDevice->SetTexture(pTextureStage, this->pTexture);
			this->pDevice->SetTexture(pMaskTextureStage, this->pMaskTexture);

			//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			//pDevice->SetTextureStageState(1, D3DTSS_RESULTARG, D3DTA_CURRENT);
			//pDevice->SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_TEXTURE);

			this->pDevice->SetTextureStageState(pTextureStage, D3DTSS_TEXCOORDINDEX, 0);

			//this->pDevice->SetTextureStageState(pTextureStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			//this->pDevice->SetTextureStageState(pTextureStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
			//this->pDevice->SetTextureStageState(pTextureStage, D3DTSS_COLOROP, D3DTOP_MODULATE);

			//this->pDevice->SetTextureStageState(pTextureStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			//this->pDevice->SetTextureStageState(pTextureStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			//this->pDevice->SetTextureStageState(pTextureStage, D3DTSS_ALPHAOP, D3DTA_TEXTURE);

			this->pDevice->SetTextureStageState(pMaskTextureStage, D3DTSS_TEXCOORDINDEX, 0);

			this->pDevice->SetTextureStageState(pMaskTextureStage, D3DTSS_COLORARG1, D3DTSS_COLORARG1);
			this->pDevice->SetTextureStageState(pMaskTextureStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
			this->pDevice->SetTextureStageState(pMaskTextureStage, D3DTSS_COLOROP, D3DTSS_COLORARG2);

			this->pDevice->SetTextureStageState(pMaskTextureStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			this->pDevice->SetTextureStageState(pMaskTextureStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			this->pDevice->SetTextureStageState(pMaskTextureStage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		}
		else
		{
			this->pDevice->SetTexture(0, this->pTexture);
			//this->pDevice->SetTexture(1, this->pMaskTexture);

			this->pDevice->SetTextureStageState(0, D3DTSS_COLORARG0, D3DTA_TEXTURE);
			this->pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			this->pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			this->pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			this->pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG0, D3DTA_TEXTURE);
			this->pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
			this->pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
			DWORD a;
			pDevice->GetRenderState(D3DRS_DESTBLEND, &a);

			this->pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		}

		this->pSprite->Draw(this->pTexture, rect, NULL, NULL, color);

		this->pSprite->End();
	}

	virtual void DrawMask(int* mask)
	{
		// Do nothing
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
			if (this->pMaskTexture != NULL)
			{
				this->pMaskTexture->Release();
			}

			this->isReleased = true;
		}
	}

	static void Reset()
	{
		for (auto tex : Sprite::texturesMap)
		{
			tex.second->Release();
		}

		Sprite::texturesMap.clear();
	}

	~Sprite()
	{
		this->Release();
	}

private:
	bool CreateSprite()
	{
		return FAILED(D3DXCreateSprite(this->pDevice, &this->pSprite));
	}

	bool LoadTexture(string& path)
	{
		int hash = Sprite::stringHasher(path);
		for (auto i : Sprite::texturesMap)
		{
			if (i.first == hash)
			{
				this->pTexture = i.second;
				return true;
			}
		}

		bool res = FAILED(D3DXCreateTextureFromFile(this->pDevice, path.c_str(), &this->pTexture));
		if (!res)
		{
			Sprite::texturesMap.insert({ hash, this->pTexture });
		}

		return res;
	}

	void GetTextureInfo()
	{
		D3DXGetImageInfoFromFile(path.c_str(), &this->InfoOriginal);
		this->pTexture->GetLevelDesc(0, &this->Info);
	}
};

hash<string> Sprite::stringHasher;
map<int, LPDIRECT3DTEXTURE9> Sprite::texturesMap;
