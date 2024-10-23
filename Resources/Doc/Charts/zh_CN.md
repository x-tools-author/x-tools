# 图表模块使用说明

图表模块可以将特定格式的数据转换为图表，目前支持的数据格式如下：

## 二进制-Y

该数据格式帧未必须为：0x00, 0x00, 0x80, 0x7f

```c
#define CH_COUNT <N>
struct Frame {
    float ch_data[CH_COUNT];
    unsigned char tail[4]{0x00, 0x00, 0x80, 0x7f};
};
```

上述伪代码中发送的数据为：

> bf 10 59 3f b1 02 95 3e 57 a6 16 be 7b 4d 7f bf 00 00 80 7f

注意：

> ch_data为小端浮点数组，里面放着需要发送的CH_COUNT个通道。tail为帧尾。

### Arduino示例代码

```c
void setup() {
 Serial.begin(115200);
}

float t = 0;
void loop() {
    t += 0.1;
    // 发送数据
    float ch[4];
    ch[0] = sin(t);
    ch[1] = sin(2*t);
    ch[2] = sin(3*t);
    ch[3] = sin(4*t);
    Serial.write((char*)ch, sizeof(float) * 4);
    // 发送帧尾
    char[4] tail = {0x00, 0x00, 0x80, 0x7f};
    Serial.write(tail, 4);
    delay(100);
}

```

## 二进制-xY

该数据格式帧未必须为：0x00, 0x00, 0x80, 0x7f

```c
#define CH_COUNT <N>
struct Frame {
    float ch_data[CH_COUNT];
    unsigned char tail[4]{0x00, 0x00, 0x80, 0x7f};
};
```

上述伪代码中发送的数据为：

> bf 10 59 3f b1 02 95 3e 57 a6 16 be 7b 4d 7f bf 00 00 80 7f

注意：

> ch_data为小端浮点数组，里面放着需要发送的CH_COUNT个通道。tail为帧尾。

## 文本-Y

```c
float y1 = 1;
float y2 = 2;
serial.print("%f, %f\n", y1, y2);
```

## 文本-XY

```c
float x1 = 1;
float y1 = 1;
float x2 = 2;
float y2 = 2;
serial.print("%f %f %f %f\n", x1, y1, x2, y2);
```
