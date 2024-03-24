#!/bin/bash  
 
files=$(git ls-files | grep '\.hh$')  
for file in $files; do  
  new_file=${file%.hh}.h  
  git mv "$file" "$new_file"  
done  