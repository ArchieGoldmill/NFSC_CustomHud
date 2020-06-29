#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

using namespace std;

#define NUM_TEX 2

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
	StageType backupState[18 * NUM_TEX];
	int backupStateLen = 0;

	StageTexture backupTexture[NUM_TEX];
	int backupTextureLen = 0;

	LPDIRECT3DDEVICE9 pDevice;

public:
	TextureStateManager(LPDIRECT3DDEVICE9 pDevice)
	{
		this->pDevice = pDevice;
	}

	void SetTexture(DWORD Stage)
	{
		StageTexture s;
		s.Stage = Stage;
		this->pDevice->GetTexture(Stage, &s.Texture);

		for (StageTexture& i : this->backupTexture)
		{
			if (i == s)
			{
				i.Texture = s.Texture;
				return;
			}
		}

		this->backupTexture[this->backupTextureLen] = s;
		this->backupTextureLen++;
	}

	void SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type)
	{
		StageType s;
		s.Stage = Stage;
		s.Type = Type;
		this->pDevice->GetTextureStageState(Stage, Type, &s.Value);

		for (StageType& i : this->backupState)
		{
			if (i == s)
			{
				i.Value = s.Value;
				return;
			}
		}

		this->backupState[this->backupStateLen] = s;
		this->backupStateLen++;
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

		this->backupStateLen = 0;
		this->backupTextureLen = 0;
	}
};