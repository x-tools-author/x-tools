#!/bin/bash

# 找到src目录下所有的*.cc和*.h文件
files=$(find src -type f \( -name "*.cc" -o -name "*.h" \))

# 使用clang-format命令来格式化文件
for srcfile in $files
do
    echo $srcfile
    clang-format -i -style=file $srcfile
done