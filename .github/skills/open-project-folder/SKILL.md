---
name: open-project-folder
description: 使用 Windows 资源管理器打开当前仓库根目录。Open the current repository root folder in Windows File Explorer.
argument-hint: ""
disable-model-invocation: true
---

# Open Project Folder / 打开项目目录

Use this skill when the user asks to open the project directory in the system file manager.
当用户提出“打开项目目录”“打开当前项目文件夹”“用资源管理器打开仓库”等请求时，也使用此技能。

## Steps / 步骤

1. Ensure the working directory is the repository root.
2. Run Windows Explorer for the current directory.

1. 确保当前工作目录是仓库根目录。
2. 执行资源管理器命令打开当前目录。

Reference command:

    explorer .

Alternative command / 备用命令:

    Start-Process explorer.exe .

## Validation / 验证

- Report that File Explorer was opened for the repository root.
- If running on a non-Windows platform, report that this skill is Windows-only and do not run the command.
- 报告已打开仓库根目录的资源管理器窗口。
- 如果不是 Windows 平台，报告该技能仅适用于 Windows，并且不执行命令。
