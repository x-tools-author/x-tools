# 瑞士军刀

![build-windows-msvc](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-windows-msvc/badge.svg)
![build-windows-mingw](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-windows-mingw/badge.svg)
![build-ubuntu](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-ubuntu/badge.svg)
![build-macos](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-macos/badge.svg)
![build-ios](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-ios/badge.svg)
![build-android](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-android/badge.svg)

|构建流程|\[构建平台\]-\[目标平台\]-\[Qt版本\]\(_编译器\)|
|----|----|
|build-windows-msvc|windows2019-windows-qt5.9.9_msvc2017_64</br>windows2019-windows-qt5.12.10_msvc2017</br>windows2019-windows-qt5.12.10_msvc2017_64</br>windows2019-windows-qt5.15.2_msvc2019</br>windows2019-windows-qt5.15.2_msvc2019_64|
|build-windows-mingw|windows2019-windows-qt5.12.10_mingw73_64</br> windows2019-windows-qt5.15.2_mingw81_64|
|build-ubuntu|ubuntu18.04-linux-qt5.9.5</br>ubuntu20.04-linux-qt5.12.8|
|build-macos|macos10.15-macos-qt5.9.9</br>macos10.15-macos-qt5.12.10</br>macos10.15-macos-qt5.15.2|
|build-ios|macos10.15-ios-qt5.12.10</br>macos10.15-ios-qt5.15.2|
|build-android|ubuntu18.04-android_x86-qt5.12.10</br>ubuntu18.04-android_armv7-qt5.12.10|

[English](../../README.md)
***************

## 如何构建工程

### 方法1 (适用于Windows，Linux及maxOS)

1. 下载Qt并安装
2. 适用QtCreator打开工程文件（QtSwissArmyKnife.pro）然后构建。

### 方法2 (仅适用于Ubuntu)

#### 安装开发环境

```(shell)
sudo apt-get update --fix-missing -y
sudo apt-get install gcc g++ -y
sudo apt-get install make git -y
sudo apt-get install libgl1-mesa-dev -y
sudo apt-get install qt5-default -y
sudo apt-get install libqt5websockets5 libqt5websockets5-dev -y
sudo apt-get install libqt5serialport5 libqt5serialport5-dev -y
sudo apt-get install libqt5charts5 libqt5charts5-dev -y
```

注意: 命令行构建无法识别QCharts模块。

#### 拉取并构建工程

从github拉取源码:

```(shell)
git clone https://gitee.com/qsaker/QtSwissArmyKnife.git
```

或者从gitee拉取源码:

```(shell)
git clone https://gitee.com/qsaker/QtSwissArmyKnife.git
```

如果想要弃用某些扩展功能，需要更新子模块:

```(shell)
git submodule update --init --recursive
```

Build the project:

```(shell)
cd QtSwissArmyKnife
qmake
make
```

## 推荐Qt版本

Qt 5.15.x，其它版本可能适用，可以自行尝试。

## 软件主界面

![MainWindow.png](MainWindow.png)

## About the Project

* 软件作者：Qter
* 联系邮箱：qsaker@qq.com
* 用户交流群: 952218522 (QQ群)
* 技术交流群 Communication：723516989 (QQ群)
* Gitee: <https://gitee.com/qsaker/QtSwissArmyKnife>
* GitHub: <https://github.com/qsaker/QtSwissArmyKnife>

***************

```txt
   ____  __
  / __ \/ /____  _____
 / / / / __/ _ \/ ___/
/ /_/ / /_/  __/ /
\___\_\__/\___/_/

```
