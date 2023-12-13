#pragma once
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <Windows.h>
#include <stdio.h>

namespace core {
class UI {
public:
  static void Initialize(const HWND window, ID3D11Device *pDevice,
                         ID3D11DeviceContext *pContext);

  static void Render();
  static void Shutdown();
};
} // namespace core
