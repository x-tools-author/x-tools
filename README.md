# xTools [![LGPL License](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0.html)

[![build-windows](https://github.com/x-tools-author/x-tools/actions/workflows/build-windows.yml/badge.svg)](https://github.com/x-tools-author/x-tools/actions/workflows/build-windows.yml)
[![build-ubuntu](https://github.com/x-tools-author/x-tools/actions/workflows/build-ubuntu.yml/badge.svg)](https://github.com/x-tools-author/x-tools/actions/workflows/build-ubuntu.yml)
[![build-macos](https://github.com/x-tools-author/x-tools/actions/workflows/build-macos.yml/badge.svg)](https://github.com/x-tools-author/x-tools/actions/workflows/build-macos.yml)
[![build-ios](https://github.com/x-tools-author/x-tools/actions/workflows/build-ios.yml/badge.svg)](https://github.com/x-tools-author/x-tools/actions/workflows/build-ios.yml)
[![build-android](https://github.com/x-tools-author/x-tools/actions/workflows/build-android.yml/badge.svg)](https://github.com/x-tools-author/x-tools/actions/workflows/build-android.yml)

*xTools* is a user-friendly, open-source, cross-platform debugging tool-set based on Qt (recommended version: Qt6.5). It offers extensive support for SerialPort, BLE, UDP, TCP, WebSocket, Modbus and CANBus debugging functionalities. Here are some notable features:

1. Data Reading and Writing: This serves as the fundamental function across all tools.
2. Timed Sending: Enables scheduled transmission of "Hello" to the device every second.
3. Auto Response: Automatically responds with "Hi" when receiving "Hello" from the device.
4. Data Transmitting: Facilitates forwarding data received from a SerialPort device to a BLE device.
5. Multiple Text Formats: Show text as bin, otc, dec, hex, ascii or urf8 etc..
6. (etc.)

> **You can get *xTools Ultimate* form [Micorsoft Store](https://www.microsoft.com/store/apps/9P29H1NDNKBB).**

![MainWindow.png](resources/images/mainwindow.png)

> [!WARNING]
> The project is not supported for building for ios yet.

## Build with Qt5

> See *resources/scripts/buildwithqt5.sh* for more information.

```shell
#!/bin/sh
apt-get update --fix-missing -y
apt install qt5-base-dev -y
apt install libqt5charts5-dev -y
apt install libqt5serialbus5-dev -y
apt install libqt5websockets5-dev -y
apt install libqt5serialport5-dev -y
mkdir buildqt5
cd buildqt5
cmake -DQT_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5 -DQt5_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5 -DCMAKE_BUILD_TYPE="Release" ../ && make
```

## Build with Qt6

> See *resources/scripts/buildwithqt6.sh* for more information.

```shell
#!/bin/sh
apt-get update --fix-missing -y
apt install qt6-base-dev -y
apt install libqt6charts6-dev -y
apt install libqt6serialbus6-dev -y
apt install libqt6websockets6-dev -y
apt install libqt6serialport6-dev -y
mkdir buildqt6
cd buildqt6
cmake -DQT_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6 -DQt6_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6 -DCMAKE_BUILD_TYPE="Release" ../ && make
```

## About the Project

<!--https://sdpro.top/blog/html/article/1016.html-->
[![stars](https://img.shields.io/github/stars/x-tools-author/x-tools?style=social)](https://img.shields.io/github/stars/x-tools-author/x-tools?style=social)
[![forks](https://img.shields.io/github/forks/x-tools-author/x-tools?style=social)](https://img.shields.io/github/forks/x-tools-author/x-tools?style=social)
[![watchers](https://img.shields.io/github/watchers/x-tools-author/x-tools?style=social)](https://img.shields.io/github/watchers/x-tools-author/x-tools?style=social)
[![star](https://gitee.com/x-tools-author/x-tools/badge/star.svg?theme=dark)](https://gitee.com/x-tools-author/x-tools/stargazers)
[![fork](https://gitee.com/x-tools-author/x-tools/badge/fork.svg?theme=dark)](https://gitee.com/x-tools-author/x-tools/members)

* Author：Qsaker
* E-mail：<x-tools@outlook.com>
* User Communication: 952218522 (QQ Group)
* Qt Technology Communication：723516989 (QQ Group)
* Gitee: <https://gitee.com/x-tools-author/x-tools>
* GitHub: <https://github.com/x-tools-author/x-tools>
