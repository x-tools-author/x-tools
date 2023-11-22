# Qt Swiss Army Knife

![build-windows-msvc](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-windows-msvc/badge.svg)
![build-windows-mingw](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-windows-mingw/badge.svg)
[![build-ubuntu](https://github.com/qsaker/QtSwissArmyKnife/actions/workflows/build-ubuntu.yml/badge.svg)](https://github.com/qsaker/QtSwissArmyKnife/actions/workflows/build-ubuntu.yml)
![build-macos](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-macos/badge.svg)
![build-ios](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-ios/badge.svg)
![build-android](https://github.com/qsaker/QtSwissArmyKnife/workflows/build-android/badge.svg)

The *Qt Swiss Army Knife* is a user-friendly, open-source, cross-platform debugging tool-set based on Qt (recommended version: Qt6.5). It offers extensive support for SerialPort, BLE, UDP, TCP, WebSocket, Modbus and CANBus debugging functionalities. Here are some notable features:

1. Data Reading and Writing: This serves as the fundamental function across all tools.
2. Timed Sending: Enables scheduled transmission of "Hello" to the device every second.
3. Auto Response: Automatically responds with "Hi" when receiving "Hello" from the device.
4. Data Transmitting: Facilitates forwarding data received from a SerialPort device to a BLE device.
5. Multiple Text Formats: Show text as bin, otc, dec, hex, ascii or urf8 etc..
6. (etc.)

![MainWindow.png](mainwindow.png)

> You can get *Qt Swiss Army Knife* form [Micorsoft Store](https://www.microsoft.com/store/apps/9P29H1NDNKBB).

## Build with Qt5

> See *buildwithqt5.sh* for more information.

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

> See *buildwithqt6.sh* for more information.

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

## Get the Source

--[![Fork me on Gitee](https://gitee.com/qsaker/QtSwissArmyKnife/widgets/widget_6.svg)](https://gitee.com/qsaker/QtSwissArmyKnife)-- </br>
[![star](https://gitee.com/qsaker/QtSwissArmyKnife/badge/star.svg?theme=dark)](https://gitee.com/qsaker/QtSwissArmyKnife/stargazers)
[![fork](https://gitee.com/qsaker/QtSwissArmyKnife/badge/fork.svg?theme=dark)](https://gitee.com/qsaker/QtSwissArmyKnife/members)

## About the Project

* Author：Qsaker
* E-mail：<qsaker@foxmail.com>
* User Communication: 952218522 (QQ Group)
* Qt Technology Communication：723516989 (QQ Group)
* Gitee: <https://gitee.com/qsaker/QtSwissArmyKnife>
* GitHub: <https://github.com/qsaker/QtSwissArmyKnife>

***

```txt
    ______     ________     __       __   ___  _______   _______   
   /    " \   /"       )   /""\     |/"| /  ")/"     "| /"      \  
  // ____  \ (:   \___/   /    \    (: |/   /(: ______)|:        | 
 /  /    )  ) \___  \    /' /\  \   |    __/  \/    |  |_____/   ) 
(: (____/ //   __/  \\  //  __'  \  (// _  \  // ___)_  //      /  
 \         \  /" \   :)/   /  \\  \ |: | \  \(:      "||:  __   \  
  \"____/\__\(_______/(___/    \___)(__|  \__)\_______)|__|  \___) 
                                                                   
```
