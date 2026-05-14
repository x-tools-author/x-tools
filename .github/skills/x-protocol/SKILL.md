---
name: x-protocol
description: xProtocol是一个自定义协议序列化/反序列化工具，用户可以编辑自己的协议格式，可以控制消息的发送，可以解析接收到的消息。支持json格式的消息和二进制格式的消息体（负载）。用户要求修改协议定义、编辑消息内容、发送消息或解析接收消息时使用此技能。
---

# xProtocol协议工具

xProtocol是一个自定义协议序列化/反序列化工具，用户可以编辑自己的协议格式，可以控制消息的发送，可以解析接收到的消息。支持json格式的消息和二进制格式的消息体（负载）。当用户要求修改协议定义、编辑消息内容、发送消息或解析接收消息时使用此技能。

## 需求文档

详见 [协议工具需求文档](../../../readme.md)；如果你无法找到此文档，终止技能执行并提示用户提供需求文档链接。

## 模块代码

协议工具的核心代码位于 `src/private/protocol/protocol` 目录下，主要包含以下文件：

```txt
./protocol
├── protocolcommon.cpp            // 协议工具公共函数实现
├── protocolcommon.h              // 协议工具公共函数声明，如数据类型转换、校验函数等
├── protocolmessageview.cpp       // 消息视图实现
├── protocolmessageview.h         // 消息视图类声明（消息负载的解析显示/用户交互）
├── protocolmessageview.ui        // 消息视图设计文件
├── protocolpropertyview.cpp      // 协议属性视图实现
├── protocolpropertyview.h        // 协议属性视图类声明
├── protocoltreefilter.cpp        // 协议树过滤器实现
├── protocoltreefilter.h          // 协议树过滤器类声明
├── protocoltreemodel.cpp         // 协议树模型实现
├── protocoltreemodel.h           // 协议树模型类声明
├── protocoltreeview.cpp          // 协议树视图实现
├── protocoltreeview.h            // 协议树视图类声明
└── readme.md                     // 协议工具功能需求文档
```
## 特别说明

你不能修改模块外的任何代码，只能修改此目录下的代码。

## 执行步骤

1. 阅读协议工具需求文档，理解协议工具的功能范围和用户需求。
2. 阅读协议工具模块代码，理解各个文件的功能和相互关系。
3. 根据用户的具体需求，定位需要修改的功能模块和相关代码文件。
4. 在协议工具模块内进行代码修改，确保修改符合项目的编码规范和风格。
5. 修改完成后，进行编译和测试，确保修改的功能正常工作且没有引入新的问题。
6. 使用 'build-xtools' 技能构建 xTools，验证修改后的协议工具功能是否通过编译。

## 其它说明

- 如果生成代码附带注释，注释要使用英文。
- 生成代码要符合项目的编码规范和风格。