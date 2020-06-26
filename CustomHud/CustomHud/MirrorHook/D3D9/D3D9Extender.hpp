// clang-format off
//
//    Copyright (C) 2020 Berkay Yigit <berkaytgy@gmail.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
//
// clang-format on

#pragma once
#include "../pch.h"
#include "../MemoryEditor/MemoryEditor.hpp"
#include "../WndProc/WndProcExtender.hpp"

// D3D9
#pragma warning(push, 0)
#include <d3d9.h>
#pragma warning(pop)
typedef HRESULT(__stdcall* Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(__stdcall* BeginScene_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(__stdcall* EndScene_t)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(__stdcall* BeginStateBlock_t)(LPDIRECT3DDEVICE9 pDevice);

namespace MirrorHookInternals::D3D9Extender {
  enum class D3D9Extension { BeginScene, EndScene, BeforeReset, AfterReset };

  std::mutex              mMutex;
  std::list<BeginScene_t> mBeginSceneExts;
  std::list<EndScene_t>   mEndSceneExts;
  std::list<Reset_t>      mBefResetExts;
  std::list<Reset_t>      mAftResetExts;

#pragma region Hooks
  // detours
  std::pair<std::unique_ptr<MemoryEditor::Editor::DetourInfo>, BeginScene_t>      mBeginScene;
  std::pair<std::unique_ptr<MemoryEditor::Editor::DetourInfo>, EndScene_t>        mEndScene;
  std::pair<std::unique_ptr<MemoryEditor::Editor::DetourInfo>, Reset_t>           mReset;
  std::pair<std::unique_ptr<MemoryEditor::Editor::DetourInfo>, BeginStateBlock_t> mBeginStateBlock;

  HRESULT __stdcall hkBeginScene(LPDIRECT3DDEVICE9 pDevice) {
    for (const auto& _ext : mBeginSceneExts) _ext(pDevice);

    mBeginScene.first->Undetour();
    auto _ret = mBeginScene.second(pDevice);
    mBeginScene.first->Detour();
    return _ret;
  }
  HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
    for (const auto& _ext : mEndSceneExts) _ext(pDevice);

    mEndScene.first->Undetour();
    auto _ret = mEndScene.second(pDevice);
    mEndScene.first->Detour();
    return _ret;
  }
  HRESULT __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
    for (const auto& _ext : mBefResetExts) _ext(pDevice, pPresentationParameters);

    mReset.first->Undetour();
    auto _ret = mReset.second(pDevice, pPresentationParameters);
    mReset.first->Detour();

    for (const auto& _ext : mAftResetExts) _ext(pDevice, pPresentationParameters);

    return _ret;
  }
  HRESULT __stdcall hkBeginStateBlock(LPDIRECT3DDEVICE9 pDevice) {
    mBeginScene.first->Undetour();
    mBeginScene.first.reset();
    mEndScene.first->Undetour();
    mEndScene.first.reset();
    mReset.first->Undetour();
    mReset.first.reset();
    mBeginStateBlock.first->Undetour();
    mBeginStateBlock.first.reset();
    auto _ret = mBeginStateBlock.second(pDevice);

    auto* _vtDevice  = *reinterpret_cast<std::uintptr_t**>(pDevice);
    mBeginStateBlock = {
        std::move(MemoryEditor::Get().Detour(_vtDevice[60], reinterpret_cast<std::uintptr_t>(&hkBeginStateBlock))),
        reinterpret_cast<BeginStateBlock_t>(_vtDevice[60])};
    mBeginScene = {
        std::move(MemoryEditor::Get().Detour(_vtDevice[41], reinterpret_cast<std::uintptr_t>(&hkBeginScene))),
        reinterpret_cast<BeginScene_t>(_vtDevice[41])};
    mEndScene = {std::move(MemoryEditor::Get().Detour(_vtDevice[42], reinterpret_cast<std::uintptr_t>(&hkEndScene))),
                 reinterpret_cast<EndScene_t>(_vtDevice[42])};
    mReset    = {std::move(MemoryEditor::Get().Detour(_vtDevice[16], reinterpret_cast<std::uintptr_t>(&hkReset))),
              reinterpret_cast<Reset_t>(_vtDevice[16])};
    return _ret;
  }
#pragma endregion
#pragma region Exported
  bool __stdcall AddExtension(D3D9Extension type, void* pExtension) {
#pragma ExportedFunction
    if (!pExtension) return false;
    std::scoped_lock<std::mutex> _l(mMutex);

    switch (type) {
      case D3D9Extension::BeginScene:
        mBeginSceneExts.push_back(reinterpret_cast<BeginScene_t>(pExtension));
        return true;
      case D3D9Extension::EndScene:
        mEndSceneExts.push_back(reinterpret_cast<EndScene_t>(pExtension));
        return true;
      case D3D9Extension::BeforeReset:
        mBefResetExts.push_back(reinterpret_cast<Reset_t>(pExtension));
        return true;
      case D3D9Extension::AfterReset:
        mAftResetExts.push_back(reinterpret_cast<Reset_t>(pExtension));
        return true;
    }
    return false;
  }
#pragma endregion

#pragma warning(push)
#pragma warning(disable : 28182)  // dereferencing null pointer

  bool Init(LPDIRECT3DDEVICE9* ppDevice) {
    if (!GetModuleHandle(TEXT("d3d9.dll"))) return false;

    while (!*ppDevice) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto _pDevice = *ppDevice;

    D3DDEVICE_CREATION_PARAMETERS _cParams;
    _pDevice->GetCreationParameters(&_cParams);

    auto* _vtDevice  = *reinterpret_cast<std::uintptr_t**>(_pDevice);
    mBeginStateBlock = {
        std::move(MemoryEditor::Get().Detour(_vtDevice[60], reinterpret_cast<std::uintptr_t>(&hkBeginStateBlock))),
        reinterpret_cast<BeginStateBlock_t>(_vtDevice[60])};
    mBeginScene = {
        std::move(MemoryEditor::Get().Detour(_vtDevice[41], reinterpret_cast<std::uintptr_t>(&hkBeginScene))),
        reinterpret_cast<BeginScene_t>(_vtDevice[41])};
    mEndScene = {std::move(MemoryEditor::Get().Detour(_vtDevice[42], reinterpret_cast<std::uintptr_t>(&hkEndScene))),
                 reinterpret_cast<EndScene_t>(_vtDevice[42])};
    mReset    = {std::move(MemoryEditor::Get().Detour(_vtDevice[16], reinterpret_cast<std::uintptr_t>(&hkReset))),
              reinterpret_cast<Reset_t>(_vtDevice[16])};

    WndProcExtender::Init(_cParams.hFocusWindow);
    return true;
  }
#pragma warning(pop)

  bool Init(HWND& hWindow) {
    auto _hLib = GetModuleHandle(TEXT("d3d9.dll"));
    if (!_hLib) return false;

    D3DPRESENT_PARAMETERS _params = {0};
    {
      _params.SwapEffect    = D3DSWAPEFFECT_DISCARD;
      _params.hDeviceWindow = hWindow;
      _params.Windowed      = FALSE;
    }

    LPDIRECT3DDEVICE9 _pDevice;
    LPDIRECT3D9       _pD3D =
        reinterpret_cast<LPDIRECT3D9(__stdcall*)(UINT)>(GetProcAddress(_hLib, "Direct3DCreate9"))(D3D_SDK_VERSION);
    if (FAILED(_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                   &_params, &_pDevice))) {
      _params.Windowed = TRUE;
      if (FAILED(_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                     &_params, &_pDevice))) {
        _pD3D->Release();
        return false;
      }
    }

    auto* _vtDevice  = *reinterpret_cast<std::uintptr_t**>(_pDevice);
    mBeginStateBlock = {
        std::move(MemoryEditor::Get().Detour(_vtDevice[60], reinterpret_cast<std::uintptr_t>(&hkBeginStateBlock))),
        reinterpret_cast<BeginStateBlock_t>(_vtDevice[60])};
    mBeginScene = {
        std::move(MemoryEditor::Get().Detour(_vtDevice[41], reinterpret_cast<std::uintptr_t>(&hkBeginScene))),
        reinterpret_cast<BeginScene_t>(_vtDevice[41])};
    mEndScene = {std::move(MemoryEditor::Get().Detour(_vtDevice[42], reinterpret_cast<std::uintptr_t>(&hkEndScene))),
                 reinterpret_cast<EndScene_t>(_vtDevice[42])};
    mReset    = {std::move(MemoryEditor::Get().Detour(_vtDevice[16], reinterpret_cast<std::uintptr_t>(&hkReset))),
              reinterpret_cast<Reset_t>(_vtDevice[16])};

    _pDevice->Release();
    _pDevice = nullptr;
    _pD3D->Release();
    _pD3D = nullptr;

    WndProcExtender::Init(hWindow);
    return true;
  }
  bool Init(const wchar_t* const szWindowTitle) {
    if (auto _hWindow = FindWindowW(0, szWindowTitle)) return Init(_hWindow);
    return false;
  }
}  // namespace MirrorHookInternals::D3D9Extender
