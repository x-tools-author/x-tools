# * argTarget        构建目标
# * argZipFile       zip压缩文件（绝对路径）
# * argPacketName    包/标识/名称
# * argPacketVersion 版本
# * argPacketSuffix  包/标识/名称后缀

set(cooked_version "${argPacketVersion}.0")
set(output_name "${argPacketName}_${cooked_version}_${argPacketSuffix}")

set(old_text ${argTarget}.zip)
set(new_text ${argZipFile})
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content template.xml) -replace '${old_text}', '${new_text}' | Set-Content template.xml")

set(old_text "${argTarget}.msix")
set(new_text "${output_name}.msix")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content template.xml) -replace '${old_text}', '${new_text}' | Set-Content template.xml")

set(old_text "${argTarget}.xml")
set(new_text "${output_name}.xml")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content template.xml) -replace '${old_text}', '${new_text}' | Set-Content template.xml")

set(old_text "${argTarget}Version")
set(new_text "${cooked_version}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content template.xml) -replace '${old_text}', '${new_text}' | Set-Content template.xml")

# * 执行打包命令，要将MsixPackagingTool.exe设置为以管理员身份运行，否则可能会不成功
# * powershell -Command "MsixPackagingTool.exe create-package --template template.xml"
execute_process(
  COMMAND powershell -Command "MsixPackagingTool.exe create-package --template template.xml"
  OUTPUT_VARIABLE tmp
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_COMMAND} -E echo "Packing finished: ${tmp}")
