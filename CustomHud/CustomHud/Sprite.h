#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>

using namespace std;

class StageTexture
{
public:
	DWORD Stage;
	IDirect3DBaseTexture9* Texture;

	bool operator==(const StageTexture& x)
	{
		return this->Stage == x.Stage;
	}
};

class StageType
{
public:
	DWORD Stage;
	D3DTEXTURESTAGESTATETYPE Type;
	DWORD Value;

	bool operator==(const StageType& x)
	{
		return this->Stage == x.Stage && this->Type == x.Type;
	}
};

class TextureStateManager
{
private:
	vector<StageType> backupState;
	vector<StageTexture> backupTexture;
	LPDIRECT3DDEVICE9 pDevice;

public:
	TextureStateManager(LPDIRECT3DDEVICE9 pDevice)
	{
		this->pDevice = pDevice;
	}

	void SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
	{
		StageTexture s;
		s.Stage = Stage;
		s.Texture = pTexture;

		bool flag = false;
		for (StageTexture& i : this->backupTexture)
		{
			if (i == s)
			{
				i.Texture = pTexture;
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			this->backupTexture.push_back(s);
		}

		this->pDevice->SetTexture(Stage, pTexture);
	}

	void SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
	{
		StageType s;
		s.Stage = Stage;
		s.Type = Type;
		s.Value = Value;

		bool flag = false;
		for (StageType& i : this->backupState)
		{
			if (i == s)
			{
				i.Value = Value;
				flag = true;
				break;
			}
		}

		if (!flag)
		{
			this->backupState.push_back(s);
		}

		this->pDevice->SetTextureStageState(Stage, Type, Value);
	}

	void Restore()
	{
		for (StageTexture& i : this->backupTexture)
		{
			this->pDevice->SetTexture(i.Stage, i.Texture);
		}

		for (StageType& i : this->backupState)
		{
			this->pDevice->SetTextureStageState(i.Stage, i.Type, i.Value);
		}

		this->backupState.clear();
		this->backupTexture.clear();
	}
};

class Sprite
{
private:
	static hash<string> stringHasher;
	static map<int, LPDIRECT3DTEXTURE9> texturesMap;

	LPDIRECT3DDEVICE9 pDevice;
	LPD3DXSPRITE pSprite;
	LPDIRECT3DTEXTURE9 pTexture;
	LPDIRECT3DTEXTURE9 pMaskTexture;
	bool isReleased;
	bool useMask;
	TextureStateManager* tsm;
protected:
	D3DXVECTOR2 maskSize;

public:
	D3DSURFACE_DESC Info;

	Sprite(LPDIRECT3DDEVICE9 pDevice, string& path, D3DXVECTOR2* maskSize = NULL)
	{
		this->isReleased = false;
		this->useMask = useMask;
		this->pDevice = pDevice;
		this->LoadTexture(path);
		this->GetTextureInfo();
		this->CreateSprite();
		if (maskSize != NULL)
		{
			this->maskSize = *maskSize;
			this->useMask = true;
			this->tsm = new TextureStateManager(this->pDevice);
			this->pDevice->CreateTexture(maskSize->x, maskSize->y, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pMaskTexture, NULL);
		}
	}

	void Draw(RECT* rect, D3DCOLOR color)
	{
		this->pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		if (this->useMask)
		{
			D3DLOCKED_RECT lockRectMask;
			this->pMaskTexture->LockRect(0, &lockRectMask, NULL, D3DLOCK_DISCARD);

			this->DrawMask((int*)lockRectMask.pBits);

			this->pMaskTexture->UnlockRect(0);

			int pTextureStage = 0;
			int pMaskTextureStage = 1;

			tsm->SetTexture(pTextureStage, pTexture);
			tsm->SetTexture(pMaskTextureStage, pMaskTexture);

			//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			//pDevice->SetTextureStageState(1, D3DTSS_RESULTARG, D3DTA_CURRENT);
			//pDevice->SetTextureStageState(0, D3DTSS_RESULTARG, D3DTA_TEXTURE);

			tsm->SetTextureStageState(pTextureStage, D3DTSS_TEXCOORDINDEX, 0);

			tsm->SetTextureStageState(pTextureStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			tsm->SetTextureStageState(pTextureStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
			tsm->SetTextureStageState(pTextureStage, D3DTSS_COLOROP, D3DTOP_MODULATE);

			tsm->SetTextureStageState(pTextureStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			tsm->SetTextureStageState(pTextureStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			tsm->SetTextureStageState(pTextureStage, D3DTSS_ALPHAOP, D3DTA_TEXTURE);

			tsm->SetTextureStageState(pMaskTextureStage, D3DTSS_TEXCOORDINDEX, 0);

			tsm->SetTextureStageState(pMaskTextureStage, D3DTSS_COLORARG1, D3DTSS_COLORARG1);
			tsm->SetTextureStageState(pMaskTextureStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
			tsm->SetTextureStageState(pMaskTextureStage, D3DTSS_COLOROP, D3DTSS_COLORARG2);

			tsm->SetTextureStageState(pMaskTextureStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			tsm->SetTextureStageState(pMaskTextureStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
			tsm->SetTextureStageState(pMaskTextureStage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

			this->pSprite->Draw(this->pTexture, rect, NULL, NULL, color);

			tsm->Restore();
		}
		else
		{
			this->pSprite->Draw(this->pTexture, rect, NULL, NULL, color);
		}

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
			if (this->tsm != NULL)
			{
				delete this->tsm;
			}

			this->pSprite->Release();
			this->pTexture->Release();
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
			delete tex.second;
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
		return FAILED(D3DXCreateSprite(this->pDevice, &pSprite));
	}

	bool LoadTexture(string& path)
	{
		int hash = Sprite::stringHasher(path);
		for (auto i : Sprite::texturesMap)
		{
			if (i.first == hash)
			{
				this->pTexture = i.second;
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
		this->pTexture->GetLevelDesc(0, &this->Info);
	}
};

hash<string> Sprite::stringHasher;
map<int, LPDIRECT3DTEXTURE9> Sprite::texturesMap;
