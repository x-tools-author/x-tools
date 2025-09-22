---
title: 安装
nav_order: 1
---

## 安装

访问[发布页面]下载所需版本即可。

[立即下载](https://github.com/x-tools-author/x-tools/releases){: .btn .btn-green }

{: .note }
> “Continuous Build”是日常构建版本，包含最新的功能，也意味着包含最新的BUG

## 如何选择文件

发布文件格式为：*xtools-[系统]-[版本]-[处理器框架].[后缀]*，例如*xtools-windows-v2025.3.6-amd64.zip*指的是软件包是于2025年3月6日构建的适用于64位windows平台。

关于文件后缀：

* zip: 绿色版软件，解压运行即可
* deb: Linux安装包，可以使用 “*sudo dpkg -i xtools-linux-v7.1.0-x86_64.deb*”来安装
* AppImage: 适用于Linux系统的单文件运行包，下载后双击运行即可，推荐Linux用户使用。
* apk: 安卓安装包
* dmg: macOS软件包

{: .important-title }
> 关于AppImage包
>
> 下载AppImage包后可能需要执行"*sudo chmod +x xtools-xxx-xxx-xxx.AppImage*"来赋予可执行权限后才能运行

{: .important-title }
> 关于deb包
>
> deb包是基于ubuntu-22.04构建的，使用于ubuntu 22.04或更新版本系统

{: .important-title }
> 关于apk包
>
> 安卓包未经测试，只是按原样提供

[发布页面]: https://github.com/x-tools-author/x-tools/releases

```shell
sudo apt install libusb-1.0-0-dev
sudo apt install libudev-dev
```