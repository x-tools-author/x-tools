---
title: Lua脚本
parent: 扩展脚本
nav_order: 1
---

## Lua脚本

> [Lua官方手册](https://www.lua.org/manual/5.4/)

![Lua脚本]({{ site.baseurl }}/assets/images/lua.png#pic_center)

## print和x_print

输出日志

## x_write

发送数据

## x_sleep

延时函数，单位为ms

## x_is_interruption_requested

退出标志，如果脚本中有循环操作，必须使用该函数来判断是否应该结束循环

## x_read

接收数据处理函数

## 定时操作示例

```lua
print("Start 'Timer' demo...")

while true do
    print("Current time: " .. os.date("%H:%M:%S"))
    x_write(os.date("%H:%M:%S") .. "\n")

    ret = x_is_interruption_requested()
    if ret then
        print("Script interrupted.")
        break
    end

    x_sleep(1000) -- Sleep for 1000 milliseconds (1 second)
end
```

> 上述程序中，x_write, x_is_interruption_requested, x_sleep都是内部注册的函数，内部已将print内容重定向至UI显示。

## 读取处理示例

```lua
print("Start 'Read' demo...")

function x_read(str)
    print("x_read:", str)
end

while true do
    ret = x_is_interruption_requested()
    if ret then
        print("Script interrupted.")
        break
    end

    x_sleep(1000) -- Sleep for 1000 milliseconds (1 second)
end
```

> 上述程序功能，将读取到的数据打印至UI
