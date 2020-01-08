# 库目录命名规则

第三方库不使用debug版本。
第三方库文件存放在工程目录的libs目录下，文件夹以库名称命名。  
下面以openssl库作为示例：  

```txt
openssl
    ├─android
    ├─linux
    │  ├─x86-1.1.1d-gcc7.3-shared
    │  ├─x86-1.1.1d-gcc7.3-static
    │  ├─x86_64-1.1.1d-gcc7.3-shared
    │  └─x86_64-1.1.1d-gcc7.3-static
    ├─macos
    ├─pi3
    └─windows
        ├─x86-1.1.1d-msvc2017-md
        ├─x86-1.1.1d-msvc2017-mt
        ├─x86_64-1.1.1d-msvc2017-md
        └─x86_64-1.1.1d-msvc2017-mt
```
