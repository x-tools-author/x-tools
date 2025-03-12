# * argRoot          工作路径
# * argTarget        构建目标
# * argTemplate      模板文件
# * ----------------------------
# * argPacketName           包/标识/名称
# * argPacketVersion        版本
# * argPackageDisplayName   显示名称
# * argRenameTarget         重命名目标,构建目标与包名不一致时可以指定该参数

if(argRenameTarget)
  execute_process(COMMAND ${CMAKE_COMMAND} -E rm -rf ${argPacketName} || true
                  WORKING_DIRECTORY ${argRoot})
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ../${argTarget} ${argPacketName}
                  WORKING_DIRECTORY ${argRoot})
  execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${argPacketName}/${argTarget}.exe
                          ${argPacketName}/${argPacketName}.exe WORKING_DIRECTORY ${argRoot})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar "cf" ${argPacketName}.zip "--format=zip"
                          "${argPacketName}" WORKING_DIRECTORY ${argRoot})
  set(argTarget ${argPacketName})
else()
  execute_process(COMMAND ${CMAKE_COMMAND} -E rm -rf ${argTarget} || true
                  WORKING_DIRECTORY ${argRoot})
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ../${argTarget} ${argTarget}
                  WORKING_DIRECTORY ${argRoot})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar "cf" ${argTarget}.zip "--format=zip"
                          "${argTarget}" WORKING_DIRECTORY ${argRoot})
endif()
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${argTemplate} ${argTarget}.xml
                WORKING_DIRECTORY ${argRoot})

set(zip_file ${argRoot}/${argTarget}.zip)
set(xml_file ${argRoot}/${argTarget}.xml)
set(msix_file
    ${argRoot}/50263Qsaker2018.${argPacketName}_${argPacketVersion}.0_x64__83fbcck3baqe6.msix)

set(old_text "xTools.msix")
set(new_text "${msix_file}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${xml_file}) -replace '${old_text}', '${new_text}' | Set-Content ${xml_file}")

set(old_text "xTools.xml")
set(new_text "${xml_file}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${xml_file}) -replace '${old_text}', '${new_text}' | Set-Content ${xml_file}")

set(old_text "xTools.zip")
set(new_text "${zip_file}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${xml_file}) -replace '${old_text}', '${new_text}' | Set-Content ${xml_file}")

set(old_text "PackageDescription=\"xTools\"")
set(new_text "PackageDescription=\"${argPackageDisplayName}\"")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${xml_file}) -replace '${old_text}', '${new_text}' | Set-Content ${xml_file}")

set(old_text "PackageDisplayName=\"xTools\"")
set(new_text "PackageDisplayName=\"${argPackageDisplayName}\"")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${xml_file}) -replace '${old_text}', '${new_text}' | Set-Content ${xml_file}")

set(old_text "50263Qsaker2018.xTools")
set(new_text "50263Qsaker2018.${argPacketName}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${xml_file}) -replace '${old_text}', '${new_text}' | Set-Content ${xml_file}")

set(old_text "1.0.0.0")
set(new_text "${argPacketVersion}.0")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${xml_file}) -replace '${old_text}', '${new_text}' | Set-Content ${xml_file}")

# * 执行打包命令，要将MsixPackagingTool.exe设置为以管理员身份运行，否则可能会不成功
# * C:\Users\user\AppData\Local\Microsoft\WindowsApps
# * C:\Users\user\AppData\Local\Packages\Microsoft.MSIXPackagingTool_8wekyb3d8bbwe\LocalState\DiagOutputDir
# * powershell -Command "MsixPackagingTool.exe create-package --template ${temperate}"
execute_process(
  COMMAND powershell -Command "MsixPackagingTool.exe create-package --template ${xml_file}"
  OUTPUT_VARIABLE tmp
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${CMAKE_COMMAND} -E echo "Packing finished: ${tmp}")
