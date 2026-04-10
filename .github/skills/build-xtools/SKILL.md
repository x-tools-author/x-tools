---
name: build-xtools
description: Build xTools or a selected X_APP variant for this repository with the project's CMake-based workflow. Use this when asked to compile, rebuild, package, or verify buildability on Windows or Linux.
argument-hint: "[target-or-xapp] [platform] [qt-path]"
disable-model-invocation: true
---

# Build xTools

Use this skill when the task is to compile this repository, rebuild after code changes, or generate a package artifact.

## Repository build facts

- The project is a Qt C++ desktop application built with CMake.
- The main build entry is [CMakeLists.txt](../../../CMakeLists.txt).
- Windows build conventions are documented in [copilot-instructions.md](../../copilot-instructions.md) and implemented in [legacy Windows script](../../../scripts/legacy/windows.bat).
- The repository supports building alternate apps through the CMake cache variable `X_APP`.
- Packaging targets include `xTools_zip`, and other package targets may exist depending on platform and toolchain.

## What to do

1. Identify the platform, compiler, Qt installation path, and whether the user wants the default app or a specific `X_APP` target.
2. Prefer the repository's existing CMake workflow instead of inventing ad hoc commands.
3. On Windows, use Ninja with the MSVC developer environment and an explicit `CMAKE_PREFIX_PATH` that points at the Qt MSVC kit.
4. On Linux, configure CMake with the correct Qt GCC path and then build the requested target.
5. If the request is only to validate compilation, build the narrowest relevant target first.
6. If the request also asks for packaging, build the package target after the main binary target succeeds.

## Windows procedure

The repository's reference flow is:

1. Load the Visual Studio x64 build environment.
2. Configure CMake with Ninja and the Qt MSVC path.
3. Build `xTools`.
4. Optionally build `xTools_zip`.

Reference command shape:

    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
    cmake -S . -B build\my-build -G Ninja -DCMAKE_PREFIX_PATH=D:/Qt/6.11.0/msvc2022_64 -DCMAKE_BUILD_TYPE=Release
    cmake --build build\my-build --target xTools
    cmake --build build\my-build --target xTools_zip

If the user asks to build a specific xApp, add `-DX_APP=<name>` during configuration.

Example:

    cmake -S . -B build\xoscilloscope -G Ninja -DCMAKE_PREFIX_PATH=D:/Qt/6.11.0/msvc2022_64 -DCMAKE_BUILD_TYPE=Release -DX_APP=xOscilloscope
    cmake --build build\xoscilloscope --target xOscilloscope

## Linux procedure

Reference command shape:

    cmake -S . -B build/linux -G Ninja -DCMAKE_PREFIX_PATH=/path/to/Qt/6.11.0/gcc_64 -DCMAKE_BUILD_TYPE=Release
    cmake --build build/linux --target xTools

For packaging on Linux, the repository documentation indicates targets such as `xTools_deb` and `xTools_appimage` when the environment supports them.

## Validation rules

- Report the exact configured build directory.
- Report the exact target that was built.
- If configuration fails, capture the missing dependency, Qt path issue, or compiler setup issue before changing anything else.
- If the build succeeds, mention the expected output artifact path or package target that was produced.

## Notes for this repository

- This project already contains build output folders under `build/`; do not assume they are reusable without checking the Qt version and generator.
- The reference Windows script removes the build directory before a fresh configure. Only do that when the user explicitly wants a clean rebuild or the build directory is clearly incompatible.
- Prefer small validation builds after code changes before attempting packaging targets.