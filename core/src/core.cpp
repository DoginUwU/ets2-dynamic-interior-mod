﻿#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "kiero.h"
#include "patches.h"
#include "ui.h"
#include <Windows.h>
#include <cstdint>
#include <d3d11.h>
#include <dxgi.h>
#include <stdio.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg,
                                              WPARAM wParam, LPARAM lParam);

typedef long(__stdcall *Present)(IDXGISwapChain *, UINT, UINT);

static Present oPresent = NULL;
uint150_t oEndScene = NULL;

HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;
ID3D11RenderTargetView *mainRenderTargetView;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam,
                          LPARAM lParam) {

  if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
    return true;
  }

  return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

long __stdcall hkPresent11(IDXGISwapChain *pSwapChain, UINT SyncInterval,
                           UINT Flags) {
  static bool init = false;

  if (!init) {
    if (SUCCEEDED(
            pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pDevice))) {
      pDevice->GetImmediateContext(&pContext);
      DXGI_SWAP_CHAIN_DESC sd;
      pSwapChain->GetDesc(&sd);
      window = sd.OutputWindow;
      ID3D11Texture2D *pBackBuffer;
      pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                            (LPVOID *)&pBackBuffer);
      pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
      pBackBuffer->Release();
      oWndProc =
          (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

      core::UI::Initialize(window, pDevice, pContext);

      init = true;
    } else {
      printf("[D3D11] Failed to get device! \n");

      return oPresent(pSwapChain, SyncInterval, Flags);
    }
  }

  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  // ImGui::Begin("Test Window");

  // VK_MENU is the ALT key
  ImGui::GetIO().MouseDrawCursor = GetAsyncKeyState(VK_MENU) & 0x80000;

  // ImGui::End();

  ImGui::Render();

  pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

  return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID param) {
  if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success) {
    oEndScene = kiero::getMethodsTable()[42];

    assert(kiero::bind(8, (void **)&oPresent, hkPresent11) ==
           kiero::Status::Success);

    core::Patches::Apply();

    printf("[Kiero] Initialized! \n");
  }

  return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
  DisableThreadLibraryCalls(hinstDLL);

  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hinstDLL, 0, 0);
    break;

  case DLL_THREAD_ATTACH:
    break;

  case DLL_THREAD_DETACH:
    break;

  case DLL_PROCESS_DETACH:
    core::UI::Shutdown();
    break;
  }

  return TRUE;
}