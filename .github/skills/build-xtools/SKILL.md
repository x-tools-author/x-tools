---
name: build-xtools
description: 使用本项目的 CMake 工作流构建 xTools 或指定的 X_APP 目标。当用户要求编译、重新构建或生成打包产物时使用此技能。
argument-hint: "[目标] [Qt路径]"
disable-model-invocation: true
---

# 构建 xTools

当任务是编译此仓库、代码修改后重新构建或生成打包产物时，使用此技能。

## 仓库构建基本信息

- 本项目是基于 CMake 构建的 Qt C++ 桌面应用程序。
- 主构建入口为 [CMakeLists.txt](../../../CMakeLists.txt)。
- 为WindowsXP及更新操作系统构建参考脚本见 [ci-build-windows-xp.bat](../../../scripts/ci-build-windows-xp.bat)。
- 为Windows7及更新操作系统构建参考脚本见 [ci-build-windows-7.bat](../../../scripts/ci-build-windows-7.bat)。
- 为其它系统构建，按后续步骤说明配置 CMake 和构建命令。

## 执行步骤

1. 确认平台、编译器、Qt 安装路径，以及用户是否需要构建默认应用或特定 `X_APP` 目标。
2. 优先使用仓库现有的 CMake 工作流，不要自行发明临时命令。
3. Windows 上使用 Ninja + MSVC 开发环境，并通过 `CMAKE_PREFIX_PATH` 指定 Qt MSVC 套件路径。
4. Linux 上使用正确的 Qt GCC 路径配置 CMake，再构建目标。
5. MacOS 上使用正确的 Qt Clang 路径配置 CMake，再构建目标。
6. 若只是验证编译，先构建最小范围的相关目标。
7. 若同时需要打包，在主二进制目标构建成功后再构建打包目标。

## Windows平台下Qt环境搜索

- 优先确认用户提供的 Qt 路径是否正确，且包含与 Visual Studio 版本匹配的.
- 优先使用提供的Qt路径，如果用户未提供路径，尝试在常见安装位置（如 `C:/Qt/`、`D:/Qt/Qt/`、`D:/512/Qt/Qt/`）搜索 Qt 版本。
- 优先使用Qt配套的cmake程序，一般位于Qt安装目录下的 `Tools/CMake/bin` 或 `Tools/CMake` 子目录中。

## Windows 构建流程

1. 加载 Visual Studio x64 构建环境。
2. 使用 Ninja 和 Qt MSVC 路径配置 CMake。
3. 构建 `xTools`。
4. 可选：构建 `xTools_zip`。

参考命令：

```bat
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cmake -S . -B build\my-build -G Ninja -DCMAKE_PREFIX_PATH=D:/Qt/Qt/6.10.3/msvc2022_64 -DCMAKE_BUILD_TYPE=Release
cmake --build build\my-build --target xTools
cmake --build build\my-build --target xTools_zip
```

若用户要求构建特定 xApp，在配置时添加 `-DX_APP=<名称>`，例如：`-DX_APP=xOscilloscope`。

示例：

```bat
cmake -S . -B build\xoscilloscope -G Ninja -DCMAKE_PREFIX_PATH=D:/Qt/Qt/6.10.3/msvc2022_64 -DCMAKE_BUILD_TYPE=Release -DX_APP=xOscilloscope
cmake --build build\xoscilloscope --target xOscilloscope
```

## Linux 构建流程

参考命令：

    cmake -S . -B build/linux -G Ninja -DCMAKE_PREFIX_PATH=/path/to/Qt/6.10.3/gcc_64 -DCMAKE_BUILD_TYPE=Release
    cmake --build build/linux --target xTools

Linux 打包目标（环境支持时）：`xTools_deb`、`xTools_appimage`。

## 验证规则

- 报告实际使用的构建目录。
- 报告实际构建的目标名称。
- 若配置失败，先定位缺失依赖、Qt 路径问题或编译器环境问题，再做其他修改。
- 若构建成功，说明预期的输出产物路径或已生成的打包目标。

## 仓库注意事项

- `build/` 下已存在构建目录，不能直接复用，需先确认 Qt 版本和生成器是否匹配。
- 参考 Windows 脚本在全新配置前会删除构建目录，仅当用户明确要求清理构建或目录明显不兼容时才执行此操作。
- 代码修改后优先做小范围验证构建，再尝试打包目标。