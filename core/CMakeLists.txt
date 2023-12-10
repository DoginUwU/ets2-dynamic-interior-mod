cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("core")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/kiero
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/kiero/minhook/include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends
)

add_library (core SHARED "core.cpp" "vendor/kiero/kiero.cpp")

add_library(MinHook STATIC
  "vendor/kiero/minhook/src/buffer.c"
  "vendor/kiero/minhook/src/hook.c"
  "vendor/kiero/minhook/src/trampoline.c"
  "vendor/kiero/minhook/src/hde/hde32.c"
  "vendor/kiero/minhook/src/hde/hde64.c"
)

add_library(ImGui STATIC
  "vendor/imgui/imgui.cpp"
  "vendor/imgui/imgui_demo.cpp"
  "vendor/imgui/imgui_draw.cpp"
  "vendor/imgui/imgui_widgets.cpp"
  "vendor/imgui/imgui_tables.cpp"
  "vendor/imgui/backends/imgui_impl_dx11.cpp"
  "vendor/imgui/backends/imgui_impl_win32.cpp"
)

target_include_directories(core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)

target_link_libraries(core PRIVATE ImGui MinHook)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET core PROPERTY CXX_STANDARD 20)
endif()
