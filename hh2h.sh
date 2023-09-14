#!/bin/bash  
  
# 获取当前目录下的所有.hh文件  
files=$(git ls-files | grep '\.hh$')  
  
# 遍历每个文件，并使用git mv命令将后缀更改为.h  
for file in $files; do  
  new_file=${file%.hh}.h  
  git mv "$file" "$new_file"  
done  
  
echo "文件后缀更改完成。"