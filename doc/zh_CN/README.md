# “瑞士军刀”

[English](../../README.md)  
***************
  
QSAK（Qt Swiss Army Knife）是一款基于Qt开源框架打造的多功能、跨平台调试工具。它集串口调试、网口调试、蓝牙调试、USB调试、HID调试、CAN总线调试、MODBUS总线调试等调试功能于一身，具备解析MAVLink、MODBUS、MQTT等协议的能力，能够运行在Windows、Linux、macOS、IOS、Android及Raspberry Pi等设备上。**项目的目标是打造软件版“瑞士军刀”。**

## 调试功能

| F \\ P         | Windows x86 | Linux x86    | MacOS        | Raspberry Pi    | Android v7   | Android v8   | Android x86  |
| -------------- | ----------- | ------------ | ------------ | --------------- | ------------ | ------------ | ------------ |
| SerialPorrt    | Y           | Y            | Y            |  N              | N            | N            | N            |
| TcpClient      | Y           | Y            | Y            |  Y              | N            | N            | N            |
| TcpServer      | Y           | Y            | Y            |  Y              | N            | N            | N            |
| UDP            | Y           | Y            | Y            |  Y              | N            | N            | N            |
| HID            | N           | N            | N            |  N              | N            | N            | N            |
| USB            | N           | N            | N            |  N              | N            | N            | N            |
| Bluetooth      | N           | N            | N            |  N              | N            | N            | N            |
| CAN            | N           | N            | N            |  N              | N            | N            | N            |
| MODBUS         | N           | N            | N            |  N              | N            | N            | N            |
| HTTP           | N           | N            | N            |  N              | N            | N            | N            |
| HTTPS          | N           | N            | N            |  N              | N            | N            | N            |

注：**F：功能 P：开发平台**

## 协议解释

* MAVLink（V1及V2）
* MODBUS
* CAN
* REST
* CoAP
* MQTT
* DDS
* AMQP
* XMPP
* JMS
* HTTP
* HTTPS

## 构建环境

| E \\ T         | Windows x86 | Linux x86    | MacOS        | ios             | Raspberry pi5   | Android        |
| -------------- | ----------- | ------------ | ------------ | ------------    | --------------- |--------------- |
| Qt Version     | 5.15        | 5.15         | Qt 5.15      |  Qt 15          | 5.15            | 5.15           |
| Compiler       | msvc 2017   | g++ 7.3      | clang 9      |  clang 9        | g++ 8.3         | g++ 4.9        |
| JDK            | /           | /            | /            | /               | /               | 1.8.0          |
| NDK            | /           | /            | /            | /               | /               | r20            |
| SDK            | /           | /            | /            | /               | /               | r24            |
| Xcode          | /           | /            | 10           | 10              | /               | /              |

注：**E：开发环境 T：目标平台**

## 主界面预览

![MainWindow.png](MainWindow.png)

## 关于项目

* 作者：wuuhii
* 邮箱：wuuhii@outlook.com
* 官网：<http://www.qsak.pro/>
* 交流：952218522 （用户交流群）
* 交流：723516989 （Qt技术交流群）

***************

```txt
                _           _
               | |         (_)
__      ___   _| |__   __ _ _
\ \ /\ / / | | | '_ \ / _` | |    sheng huo bu zhi yan qian de gou qie, hai you yuan fang de gou qie.
 \ V  V /| |_| | | | | (_| | |           but do not be upset, coding will make you happy.
  \_/\_/  \__,_|_| |_|\__,_|_|                                                               --Confucius
```
