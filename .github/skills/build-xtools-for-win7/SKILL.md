---
name: build-xtools-for-win7
description: 使用预定义脚本构建兼容版本（win7或更新操作系统）。当用户为Windows 7或更新系统构建时，优先使用此技能。
argument-hint: "[]"
disable-model-invocation: true
---

# 构建 xTools

当任务是为 Windows 7 或更新系统构建此仓库时，优先使用此技能。

## 执行步骤

1. 确认用户需要为 Windows 7 或更新系统构建。
2. 使用仓库提供的 [ci-build-windows-7.bat](../../../scripts/ci-build-windows-7.bat) 脚本构建。

参考命令：

```bat
call scripts\ci-build-windows-7.bat
```

注意：

脚本中存在 `pause()`，即构建完成后会等待用户按任意键继续。你要自动化这个过程，确保在运行脚本时模拟按键输入以继续执行，或者修改脚本去掉 `pause()` 以适应自动化构建流程。

## 检查结果

* 构建结束后不要运行生成的可执行文件
* 如果用户成功运行了脚本，且 `bin/5.15.14/Windows/Release/xTools.exe` 可执行文件生成，报告构建成功。
* 如果用户未成功运行脚本，或生成的可执行文件缺失，报告构建失败，报告构建失败且终止任务。