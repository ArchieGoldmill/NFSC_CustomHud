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

namespace MirrorHookInternals::WndProcExtender {
  std::mutex         mMutex;
  HWND               mWindowHandle;
  std::list<WNDPROC> mExtensions;

#pragma region Hooks
  WNDPROC origWndProc;

  LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static LRESULT stickyRetVal = -1;
    for (const auto& ext : mExtensions) stickyRetVal &= ext(hWnd, uMsg, wParam, lParam);
    if (stickyRetVal != -1) {
      LRESULT ret  = stickyRetVal;
      stickyRetVal = -1;
      return ret;
    }
    return CallWindowProc(origWndProc, hWnd, uMsg, wParam, lParam);
  }
#pragma endregion
#pragma region Exported
  void __stdcall AddExtension(void* pExtension) {
#pragma ExportedFunction
    if (!pExtension) return;
    std::scoped_lock<std::mutex> _l(mMutex);

    mExtensions.push_back(reinterpret_cast<WNDPROC>(pExtension));
  }
  HWND __stdcall GetWindowHandle() {
#pragma ExportedFunction
    return mWindowHandle;
  }
#pragma endregion

  void Init(HWND& hWindow) {
    if (!origWndProc) {
      mWindowHandle = hWindow;
      origWndProc =
          reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hkWndProc)));
    }
  }
}  // namespace MirrorHookInternals::WndProcExtender
