# TFTP

你是一名资深的Qt软件工程师，现在需要基于Qt实现一个TFTP Client和一个TFTP Server小工具。这两个小工具的UI文件已经设计好了，现在需要你基于ui文件实现功能逻辑。具体需求如下：

## 目录框架

```txt
tftp
├── client
├── common
└── server
```

1. common: 客户端和服务器公用的相关代码
2. client: 客户端相关代码
3. server: 服务器相关代码

## 限制

1. 你不能修改这个文件：tftp/client/tftpclient.ui
2. 你不能修改这个文件：tftp/server/tftpserver.ui
3. 你不能修改tftp文件夹外除`assistantfactory.h`和`assistantfactory.cpp`外的文件
4. 你只能在tftp文件夹内添加/修改相关代码

## 客户端

1. 文件上传：用户选择本地文件，点击上传按钮开始上传，上传时要显示上传进度。
2. 文件下载：用户填写远程文件路径和本地保存路径，点击下载开始下载文件并保存在本地，需要显示下载进度

总的来说，客户端的需求就是实现`tftp/client/tftpclient.ui`这个ui文件中的相关功能。

## 服务器

1. 文件接收：接受客户端发送的文件，保存在服务器目录
2. 文件发送：发送客户端请求的文件
3. 文件接收/发送要显示相关日志（log模块）
4. 文件接收/发送要显示进度（session模块）

总的来说，客户端的需求就是实现`tftp/client/tftpserver.ui`这个ui文件中的相关功能。

## 其他

1、如果有需要，你可以引用`common/x.h`中的相关接口。
2、Qt相关模块已经引入，你不需要考虑模块引入相关功能。
3、cmake回自动将tftp功能模块添加到工程中，你无需考虑cmake引入源码的配置相关问题。