#pragma once
#include <Windows.h>
#include <stdio.h>

namespace core {
class Patches {
public:
  static void Apply();

private:
  static void DisableSetCursorPos();
  static void CreateTerminal();
};
} // namespace core