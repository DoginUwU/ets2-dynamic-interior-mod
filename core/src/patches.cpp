#include "patches.h"

namespace core {
void Patches::Apply() {
  CreateTerminal();
  DisableSetCursorPos();
}

void Patches::DisableSetCursorPos() {
  PVOID pSetCursorPos =
      GetProcAddress(GetModuleHandle("user32.dll"), "SetCursorPos");
  DWORD dwOldProtect = 0;
  VirtualProtect(pSetCursorPos, 2, PAGE_EXECUTE_READWRITE, &dwOldProtect);
  *(BYTE *)pSetCursorPos = 0xC3;
  VirtualProtect(pSetCursorPos, 2, dwOldProtect, &dwOldProtect);
}

void Patches::CreateTerminal() {
  AllocConsole();

  FILE *f = new FILE();
  freopen_s(&f, "CONOUT$", "w", stdout);
}
} // namespace core
