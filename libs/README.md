# 库目录命名规则

第三方库不使用debug版本。
第三方库文件存放在工程目录的libs目录下，文件夹以库名称命名。  
下面以openssl库作为示例：  

```txt
openssl
    └─1.1.1g
        ├─linux
        │  ├─amd64
        │  ├─armv7
        │  ├─armv8
        │  └─x86
        └─windows
            ├─amd64
            └─x86
```
