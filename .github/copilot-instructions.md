# xTools – AI Coding Agent Instructions

These notes help AI agents work productively in this repo by documenting the real build, packaging, architecture, and extension patterns used here. Keep changes minimal and aligned with existing CMake macros and Qt patterns.

## Big Picture
- Core app: xTools is a Qt C++ desktop app with modular I/O pages and utility “assistants”. Entry at [src/main.cpp](../src/main.cpp); app bootstraps in [src/application.{h,cpp}](../src/application.h).
- UI shell: `MainWindow` (grid of up to 4 `Page` panes) manages layout and persistence; see [src/mainwindow.{h,cpp}](../src/mainwindow.h) and [src/page/](../src/page).
- Multi-app mode: CMake selects a single “xApp” (xTools, xSvg, xCode, xDebug, xFlow, etc.) via `X_APP`; see [xapps/xApp.cmake](../xapps/xApp.cmake). Each xApp has its own CMake and sources (e.g. [xapps/xoscilloscope/xOscilloscope.cmake](../xapps/xoscilloscope/xOscilloscope.cmake)).
- CMake first: Project orchestration lives in [CMakeLists.txt](../CMakeLists.txt) and custom modules in [cmake/](../cmake). Prefer using provided macros over ad‑hoc commands.

## Build Workflows
- Windows (MSVC + Ninja + Qt): Run [scripts/windows.bat](../scripts/windows.bat). It calls `vcvarsall.bat`, sets `CMAKE_PREFIX_PATH` to Qt, configures Ninja, builds targets `xTools` and `xTools_zip`. Qt DLLs are deployed via `windeployqt` through `x_deploy_qt()`.
- Linux (GCC/Clang + Ninja): See [scripts/linux.sh](../scripts/linux.sh). Configure with `-DCMAKE_PREFIX_PATH=/path/to/Qt/<ver>/gcc_64`, then build `xTools`, `xTools_deb`, `xTools_appimage`.
- Android: See [scripts/android.bat](../scripts/android.bat). Uses `qt-cmake`, `androiddeployqt` and requires `JAVA_HOME`, `ANDROID_NDK_ROOT`, `ANDROID_SDK_ROOT`.
- macOS: Bundling/DMG/PKG are produced via CMake: `x_generate_dmg()` and `x_generate_pkg()` (see [CMakeLists.txt](../CMakeLists.txt)). Optional signing IDs can be set in `private.cmake`.

## Key CMake Macros and Targets
- General helpers in [cmake/x.cmake](../cmake/x.cmake): `x_add_executable()`, `x_generate_zip()`, git metadata injection, resource deployment.
- Qt: [cmake/x_qt*.cmake](../cmake/x_qt.cmake) manage optional Qt modules; `x_deploy_qt()` wraps platform deploy tools ([cmake/x_qt_deploy.cmake](../cmake/x_qt_deploy.cmake)).
- Packaging:
  - Windows: `xTools_zip` (ZIP), `xTools_msix` (MSIX, see [cmake/msix/msix.cmake](../cmake/msix/msix.cmake)), `xTools_installer` (Qt IFW, see [cmake/qifw/qifw.cmake](../cmake/qifw/qifw.cmake)).
  - Linux: `xTools_deb`, `xTools_appimage` (see [cmake/linux/](../cmake/linux)).
  - macOS: DMG/PKG via `x_generate_dmg`/`x_generate_pkg`.

## Important Options (CMake cache)
- `X_APP`: select which xApp to build (defaults to `xTools`). See [xapps/xApp.cmake](../xapps/xApp.cmake).
- Feature flags: `X_ENABLE_TFTP`, `X_ENABLE_HID` (see [cmake/x_3rd_hidapi.cmake](../cmake/x_3rd_hidapi.cmake)), `X_ENABLE_SERIALBUS` (auto, see [cmake/x_qt_serialbus.cmake](../cmake/x_qt_serialbus.cmake)).
- Tooling: `X_USING_VCPKG` (vcpkg toolchain), `X_STATIC_VC_LIBS` (static MSVC runtime with static Qt), `X_MAGIC` (developer-only visual tweaks), `X_COPY_COMPILE_COMMANDS` (copy `compile_commands.json`).
- Qt path: Always pass `-DCMAKE_PREFIX_PATH` to the correct Qt install (e.g., `msvc2022_64`, `gcc_64`).

## 3rd-Party and Modules
- Managed via [cmake/x_3rd.cmake](../cmake/x_3rd.cmake) with per-lib modules (Lua, HIDAPI, Zint, QXlsx, qmdnsengine, mongoose, etc.). Macros `x_auto_import_package*` auto-extract/build and install into per-Qt/version paths under `libs/`.
- Qt optional modules (SerialPort, SerialBus, Bluetooth, WebSockets, Charts, Location) are detected and files are removed/defs set when absent; see [cmake/x_qt_serialport.cmake](../cmake/x_qt_serialport.cmake) and peers.

## i18n
- Languages are declared in [cmake/x_qt_linguist.cmake](../cmake/x_qt_linguist.cmake). Use `x_generate_translations(<target>)` for app `.ts/.qm` and `x_using_x_tools_translations(<target>)` for shared translations. Output goes to `translations/` next to binaries (or inside `.app` on macOS).

## Code Patterns & Extension Points
- App shell: `Application` derives from `xApp` for logging, HDPI, language/style setup; see [src/application.cpp](../src/application.cpp).
- Main window: `MainWindow` derives from `xUi`; manages `Page` grids, persistence to JSON, tray integration, and help/about; see [src/mainwindow.cpp](../src/mainwindow.cpp).
- Pages: `src/page/` contains device/panel/preset/protocol submodules aggregated into page panes.
- Assistants: Add small tools under `src/tools/<name>/` and register via [src/tools/assistantfactory.cpp](../src/tools/assistantfactory.cpp) using the `addAssistant<T>()` pattern.

## Practical Examples
- Configure on Windows (MSVC + Ninja):
  - Open a VS x64 dev shell or call `vcvarsall.bat`.
  - Configure: `cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.8.x/msvc2022_64" -DCMAKE_BUILD_TYPE=Release`
  - Build: `cmake --build build --target xTools`
  - Package: `cmake --build build --target xTools_zip`
- Build a specific xApp: add `-DX_APP=xOscilloscope` (requires Qt 6.8+ for xApps).

## Conventions
- Prefer existing CMake macros (deploy, packaging, 3rd-party) over duplicating logic.
- Keep new components consistent with existing folder boundaries (`common/`, `utilities/`, `tools/`, `page/`).
- When adding feature flags, mirror existing `X_ENABLE_*` usage (defs + selective source removal) in a dedicated `cmake/x_*.cmake` module.

If any of the above feels incomplete (e.g., signing IDs on macOS, custom Qt paths, or platform-specific gotchas), tell us what’s missing and we’ll refine this file.
