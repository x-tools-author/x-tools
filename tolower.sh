#!/bin/bash

# 检查是否传入了路径参数
if [ -z "$1" ]; then
  echo "请提供一个路径参数。"
  exit 1
fi

# 获取路径参数
TARGET_DIR="$1"

# 检查路径是否存在
if [ ! -d "$TARGET_DIR" ]; then
  echo "指定的路径不存在。"
  exit 1
fi

# 递归地将目录和文件名称改为小写
find "$TARGET_DIR" -depth | while read -r FILE; do
  # 获取文件的目录和名称
  DIR=$(dirname "$FILE")
  BASENAME=$(basename "$FILE")
  
  # 将名称改为小写
  LOWERCASE_BASENAME=$(echo "$BASENAME" | tr 'A-Z' 'a-z')
  
  # 如果名称已经是小写，则跳过
  if [ "$BASENAME" != "$LOWERCASE_BASENAME" ]; then
    # 重命名文件或目录
    # git mv "$FILE" "$DIR/$LOWERCASE_BASENAME"
    
    # 使用git命令处理更改
    git mv "$FILE" "$DIR/$LOWERCASE_BASENAME"
  fi
done

echo "所有文件和目录名称已改为小写。"