---
title: xCoAP
nav_order: 200
---

# CoAP Option 详解

CoAP（Constrained Application Protocol）是专门为受限环境（如 IoT 设备）设计的应用层协议，它的 Option 机制类似于 HTTP 的头部（Header），但更加轻量和灵活。

## Option 的基本结构

格式

```txt
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| Option Delta | Option Length |    Option Value (变长)          ...
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
```

字段说明：

* Option Delta（4位）：当前 Option 与前一个 Option 的编号差值（差分编码，节省空间）
* Option Length（4位）：Option Value 的长度（0-12字节直接表示，13-268字节用扩展字节）
* Option Value（变长）：Option 的实际数据

差分编码示例
```txt
假设要编码 Option 编号：3, 11, 11, 27
实际编码的 Delta 值：3, 8, 0, 16
计算方式：
  第1个: 3 - 0 = 3
  第2个: 11 - 3 = 8
  第3个: 11 - 11 = 0
  第4个: 27 - 11 = 16
```

## 标准 Option 类型（RFC 7252 + 扩展）

核心 Option（RFC 7252）

|Option编号|名称|格式|说明|示例值|
|------|----|----|----|----|
|1|If-Match|opaque|条件请求，ETag 匹配|	0x01 0x02 0x03|
|3|Uri-Host|string|主机名|	"example.com"|
|4|ETag|opaque|实体标签|	0x8a 0x09 0x3c|
|5|If-None-Match|empty|条件请求（无匹配时处理）|	（空）|
|6|Uri-Port|uint|端口号|5683|
|7|Location-Path|	string|资源位置路径|	"sensors/temp"|
|8|Uri-Path|	string|资源路径|	"temp"|
|11|Content-Format|	uint|内容格式（Media Type）|	40 (application/json)|
|12|Max-Age|	uint|	响应最大有效期（秒）|	60|
|14|Uri-Query|	string|	查询参数|	"unit=celsius"|
|15|Accept|	uint|	客户端接受的格式|	50 (application/cbor)|
|17|Location-Query|	string|	位置查询参数|	"id=123"|
|20|Proxy-Uri|	string|	代理 URI|	"coap://proxy/res"|
|35|Proxy-Scheme|	string|	代理方案|	"coap"|
|39|Size1|	uint|	请求体大小估计|	256|

## 常用 Content-Format 值

```text
0  - text/plain
40 - application/json
41 - application/cbor
42 - application/exi
47 - application/link-format
50 - application/octet-stream
60 - application/cose; cose-type="cose-encrypt"
```

##  Option 在消息中的组织规则
排序规则
Options 必须按照 Option 编号升序排列：

```text
Uri-Path (8) → Uri-Query (14) → Content-Format (11) ❌ 错误！
Uri-Path (8) → Content-Format (11) → Uri-Query (14) ✅ 正确
```

重复规则
Option 类型	是否可重复	处理方式
Uri-Path, Uri-Query	是	拼接使用（如: /path1/path2?q1=a&q2=b）
ETag, If-Match	是	列表处理
其他大多数 Option	否	最后一个有效
Uri-Path 和 Uri-Query 的特殊处理
text
GET coap://server.com/sensors/temp?unit=c&limit=10

实际编码：
Uri-Path: "sensors"  (Option 8, Delta=8)
Uri-Path: "temp"     (Option 8, Delta=0)  ← 相同编号，Delta=0
Uri-Query: "unit=c"  (Option 14, Delta=6)
Uri-Query: "limit=10"(Option 14, Delta=0)

## 实际消息示例

### 请求消息

```text
CoAP Request: GET /sensors/temperature?unit=celsius
Ver: 1
Type: CON (0)
Code: 0.01 (GET)
MsgID: 0x7d34
Token: 0x31 0x32 0x33 0x34

Options:
  Option #3 (Uri-Host): "iot.example.com"
  Option #8 (Uri-Path): "sensors"
  Option #8 (Uri-Path): "temperature"
  Option #14 (Uri-Query): "unit=celsius"
  Option #11 (Content-Format): 40 (application/json)

Payload: (空)
```

### 字节流表示：

```text
0x44 0x01 0x7d 0x34 0x31 0x32 0x33 0x34
0x33 0x65 0x69 0x6f 0x74 0x2e 0x65 0x78 0x61 0x6d 0x70 0x6c 0x65 0x2e 0x63 0x6f 0x6d
0x88 0x07 0x73 0x65 0x6e 0x73 0x6f 0x72 0x73
0x00 0x0b 0x74 0x65 0x6d 0x70 0x65 0x72 0x61 0x74 0x75 0x72 0x65
0x8d 0x0d 0x75 0x6e 0x69 0x74 0x3d 0x63 0x65 0x6c 0x73 0x69 0x75 0x73
0xb1 0x28
```
### 响应消息

```text
CoAP Response: 2.05 Content
Ver: 1
Type: ACK (2)
Code: 2.05 (Content)
MsgID: 0x7d34

Options:
  Option #12 (Max-Age): 60
  Option #11 (Content-Format): 40 (application/json)

Payload: {"temp": 22.5, "unit": "celsius"}
```

## 扩展 Option（常见 RFC）

|RFC|	新增 Option|	说明|
|----|--------------|----------------|
|RFC 7959|Block1 (27), Block2 (23)|	分块传输，用于大数据
|RFC 8323|TCP_Connection|	CoAP over TCP 支持
|RFC 8613|OSCORE (9)|	对象安全（加密/完整性）
|RFC 8974|Hop-Limit (16)|	跳数限制（类似 TTL）

### Block2 Option 示例（分块传输）

```text
请求第1块（0-1023字节）:
Block2: 0x08  (Size=1024, More=1, Num=0)
→ 0b00001000 = More=1, Num=0, Size=6 (2^(4+6)=1024)

请求第2块（1024-2047字节）:
Block2: 0x18  (Size=1024, More=1, Num=1)
→ 0b00011000 = More=1, Num=1, Size=6

最后一块:
Block2: 0x28  (Size=1024, More=0, Num=2)
→ 0b00101000 = More=0, Num=2, Size=6
```

##  Option 的设计优势

1. 紧凑编码：差分编码 + 长度优化，最小化开销
2. 可扩展性：自定义 Option（编号 256+ 为企业私有）
3. 顺序处理：简化解析器实现
4. 与 HTTP 映射：便于与 Web 系统集成

## 开发注意事项

### 解析时：

必须按顺序处理 Option

注意 Delta 的重构：current_number = previous + delta

处理重复 Option 的语义

### 生成时：

排序 Option 编号

为重复 Option 设置 Delta=0

合理选择 Content-Format

### 自定义 Option：

```c
// 自定义 Option 编号应 >= 256
#define MY_CUSTOM_OPTION 256

// 注册到 CoAP 库
coap_add_option(response,
                COAP_OPTION_NUM(MY_CUSTOM_OPTION),
                (uint8_t*)"data", 4);
```

CoAP Option 的设计体现了物联网协议的核心理念：'在极简的基础上提供足够的灵活性'，是理解 CoAP 协议的关键部分。