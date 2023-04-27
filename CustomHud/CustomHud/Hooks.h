#pragma once
#include <d3d9.h>
#include <d3dx9.h>

HRESULT __stdcall hookedReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
HRESULT __stdcall hookedEndScene(IDirect3DDevice9* pDevice);

typedef HRESULT(__stdcall* Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(__stdcall* EndScene_t)(LPDIRECT3DDEVICE9 pDevice);

inline Reset_t oReset;
inline EndScene_t oEndScene;