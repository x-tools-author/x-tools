#!/bin/bash  
  
files=$(git ls-files | grep '\.pri$')  
for file in $files; do  
  git rm "$file"  
done