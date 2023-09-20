#!/bin/bash  
  
repo_root=$(git rev-parse --show-toplevel)  
for file in $(find "$repo_root" -name "*.cc" -o -name "*.h" -o -name "*.ui"); do  
    lowercase_file=$(echo "$file" | tr '[:upper:]' '[:lower:]')  
    if [ "$file" != "$lowercase_file" ]; then  
        git mv "$file" "$lowercase_file" -f
        echo "Rename $file to $lowercase_file"  
    fi  
done