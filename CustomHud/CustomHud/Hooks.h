#pragma once
#include <d3d9.h>
#include <d3dx9.h>

void __stdcall hookedReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
void __stdcall hookedEndScene(IDirect3DDevice9* pDevice);