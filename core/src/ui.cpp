#include "ui.h"

namespace core {
void UI::Initialize(const HWND window, ID3D11Device *pDevice,
                    ID3D11DeviceContext *pContext) {
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
  ImGui_ImplWin32_Init(window);
  ImGui_ImplDX11_Init(pDevice, pContext);

  printf("[D3D11] Initialized! \n");
}

void UI::Shutdown() {
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  printf("[D3D11] Shutdown! \n");
}
} // namespace core
