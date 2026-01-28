CoAP消息结构概述
CoAP（受限应用协议）是一种轻量级的类HTTP协议，专为物联网设备设计。其消息结构紧凑，主要分为4层：

1. 整体结构图
text
┌─────────────────────────────────────────────┐
│                CoAP Message                 │
├─────────────────────────────────────────────┤
│           CoAP Header (4-72字节)            │
├─────────────────────────────────────────────┤
│               Options (可变)                 │
├─────────────────────────────────────────────┤
│               Payload (可选)                │
└─────────────────────────────────────────────┘
2. 详细结构分解
A. 固定头部（4字节）
text
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Ver| T |  TKL  |      Code     |          Message ID           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
│  Token (可选，长度由TKL指定)                                   │
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
字段说明：

Ver (2位)：版本号，始终为01（CoAP v1）

T (2位)：消息类型

00 = CON（Confirmable）- 需要确认

01 = NON（Non-confirmable）- 不需要确认

10 = ACK（Acknowledgment）- 确认响应

11 = RST（Reset）- 复位

TKL (4位)：令牌长度（0-8字节）

Code (8位)：请求方法或响应状态

请求：0.01 GET, 0.02 POST, 0.03 PUT, 0.04 DELETE

响应：2.05 Content, 4.04 Not Found, 5.00 Internal Error

Message ID (16位)：消息标识，用于匹配请求/响应

B. 令牌（Token）
长度：0-8字节（由TKL指定）

用途：关联请求和响应，类似于HTTP的请求ID

示例：客户端生成的随机数，用于匹配异步响应

C. 选项（Options）
text
选项结构：
┌─────────────┬─────────────┬─────────────┬─────────────┐
│ Option Delta │ Option Len  │  Delta扩展  │ 长度扩展    │
├─────────────┼─────────────┼─────────────┼─────────────┤
│              Option Value (可变长度)                     │
└─────────────┴─────────────┴─────────────┴─────────────┘
常用选项：

选项号	名称	格式	描述
1	If-Match	opaque	条件请求
3	Uri-Host	string	主机名
4	ETag	opaque	实体标签
5	If-None-Match	empty	条件请求
6	Observe	uint	观察选项
7	Uri-Port	uint	端口号
8	Location-Path	string	位置路径
11	Uri-Path	string	资源路径
12	Content-Format	uint	内容格式
14	Max-Age	uint	最大缓存时间
15	Uri-Query	string	查询参数
17	Accept	uint	可接受内容类型
20	Location-Query	string	位置查询
23	Block2	uint	响应块传输
27	Block1	uint	请求块传输
28	Size2	uint	响应大小
35	Proxy-Uri	string	代理URI
39	Proxy-Scheme	string	代理方案
60	Size1	uint	请求大小
选项示例：

text
URI: coap://server.com:5683/temp?unit=celsius
对应选项：
Uri-Host: "server.com"
Uri-Port: 5683
Uri-Path: "temp"
Uri-Query: "unit=celsius"
D. Payload标记
固定值：0xFF（1字节）

作用：分隔选项和payload

注意：只有存在payload时才有此标记

E. Payload
格式：任意二进制数据

长度：从消息末尾到payload标记

最大限制：受UDP MTU限制，通常1024字节

3. 消息类型示例
示例1：GET请求（CON消息）
text
CON [0x7d34]   ← 消息类型+消息ID
GET /temp      ← 方法+路径
Token: 0x41    ← 令牌
Uri-Host: example.com
Uri-Port: 5683
Observe: 0     ← 注册观察者
二进制表示（简化）：

text
0x40 0x01 0x7d 0x34  ← 头部: CON, TKL=1, GET, MID=0x7d34
0x41                 ← 令牌 (1字节)
0xbb 0x65 0x78 ...  ← 选项: Uri-Host, Uri-Path等
示例2：响应（ACK消息）
text
ACK [0x7d34]       ← 确认之前的消息
2.05 Content       ← 响应码
Token: 0x41        ← 匹配请求令牌
Content-Format: text/plain
Max-Age: 60
Payload: "22.5"    ← 温度数据
示例3：块传输请求
text
CON [0x8a23]
PUT /sensor-data
Block1: 2/M/1024   ← 第2块，更多块，块大小1024
Payload: [数据块]
4. 消息交换模式
A. 可靠传输（CON/ACK）
text
客户端                  服务器
  |--- CON [MID=123] --->|   ← 可确认请求
  |<--- ACK [MID=123] ---|   ← 确认收到
  |<--- CON [MID=456] ---|   ← 响应（可能需要确认）
  |--- ACK [MID=456] --->|   ← 确认响应
B. 不可靠传输（NON）
text
客户端                  服务器
  |--- NON [MID=789] --->|   ← 不可确认请求
  |<--- NON [MID=abc] ---|   ← 响应（也不确认）
C. 分离响应
text
客户端                  服务器
  |--- CON [MID=111] --->|
  |<--- ACK [MID=111] ---|   ← 先确认请求
  ... (处理时间) ...
  |<--- CON [MID=222] ---|   ← 延迟发送响应
  |--- ACK [MID=222] --->|   ← 确认响应
5. 协议特点总结
特性	CoAP	HTTP对比
传输层	UDP	TCP
头部大小	4-72字节	通常100+字节
消息类型	CON/NON/ACK/RST	无对应概念
方法	GET/POST/PUT/DELETE	相同+更多
状态码	类似HTTP但精简	完整
发现	支持资源发现	不支持
观察	内置观察模式	需要WebSocket/SSE
块传输	RFC 7959标准	分块编码
6. 实际消息解析示例
假设原始UDP数据：

text
44 01 00 00 39 74 65 6d 70 3f 75 6e 69 74 3d 63
解析：

text
44        → 01000100
          → Ver=01, T=00(CON), TKL=1
01        → Code=0.01 (GET)
00 00     → Message ID=0
39        → 令牌=0x39
74 65 6d 70 → Uri-Path: "temp"
3f        → Uri-Query开始
75 6e 69 74 3d 63 → "unit=c"
对应请求：

text
CON [0x0000]
GET /temp?unit=c
Token: 0x39
7. 与HTTP的映射关系
CoAP设计时考虑了与HTTP的互操作性：

方法代码映射：GET(0.01) ↔ HTTP GET

状态码映射：2.05(Content) ↔ 200 OK

选项映射：Content-Format ↔ Content-Type

URI结构相同：coap://host/path ↔ http://host/path

这种设计使得网关可以相对容易地在CoAP和HTTP之间进行转换。

CoAP的消息结构体现了其设计哲学：在保持功能完整性的同时，最大限度地减少开销，使其适合在资源受限的物联网设备上运行。