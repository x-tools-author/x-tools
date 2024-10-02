# * argTarget:        构建目标名称
# * argVersion:       版本号，如：1.0.0
# * argDataDir:       编译输出目录
# * argTemperateDir:  安装模板目录 *
# * argBinarycreator: Qt Installer Framework
# * argRootDir:       工作目录
# * argIcon:      图标文件

execute_process(COMMAND ${CMAKE_COMMAND} -E rm -rf ${argRootDir} "||" ${CMAKE_COMMAND} -E true)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${argRootDir})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${argTemperateDir} ./ COMMAND_ECHO STDOUT
                WORKING_DIRECTORY ${argRootDir})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${argDataDir} ./packages/all/data
                WORKING_DIRECTORY ${argRootDir})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${argIcon}
                        ./packages/all/data/icon.ico WORKING_DIRECTORY ${argRootDir})
# --------------------------------------------------------------------------------------------------
set(config_xml config/config.xml)
set(old_text "argConfigName")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${config_xml}) -replace '${old_text}', '${new_text}' | Set-Content ${config_xml}"
  WORKING_DIRECTORY ${argRootDir})
set(old_text "argConfigVersion")
set(new_text "${argVersion}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${config_xml}) -replace '${old_text}', '${new_text}' | Set-Content ${config_xml}"
  WORKING_DIRECTORY ${argRootDir})
set(old_text "argConfigTitle")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${config_xml}) -replace '${old_text}', '${new_text}' | Set-Content ${config_xml}"
  WORKING_DIRECTORY ${argRootDir})
set(old_text "argConfigStartMenuDir")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${config_xml}) -replace '${old_text}', '${new_text}' | Set-Content ${config_xml}"
  WORKING_DIRECTORY ${argRootDir})
set(old_text "argConfigTargetDir")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${config_xml}) -replace '${old_text}', '${new_text}' | Set-Content ${config_xml}"
  WORKING_DIRECTORY ${argRootDir})
# --------------------------------------------------------------------------------------------------
set(package_xml packages/all/meta/package.xml)
set(old_text "argPackageDisplayName")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${package_xml}) -replace '${old_text}', '${new_text}' | Set-Content ${package_xml}"
  WORKING_DIRECTORY ${argRootDir})
set(old_text "argPackageDescription")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${package_xml}) -replace '${old_text}', '${new_text}' | Set-Content ${package_xml}"
  WORKING_DIRECTORY ${argRootDir})
set(old_text "argPackageVersion")
set(new_text "${argVersion}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${package_xml}) -replace '${old_text}', '${new_text}' | Set-Content ${package_xml}"
  WORKING_DIRECTORY ${argRootDir})

string(TIMESTAMP current_date "%Y-%m-%d")
set(old_text "argPackageReleaseDate")
set(new_text ${current_date})
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${package_xml}) -replace '${old_text}', '${new_text}' | Set-Content ${package_xml}"
  WORKING_DIRECTORY ${argRootDir})
# --------------------------------------------------------------------------------------------------
set(installscript packages/all/meta/installscript.js)
set(old_text "argConfigTargetDir")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${installscript}) -replace '${old_text}', '${new_text}' | Set-Content ${installscript}"
  WORKING_DIRECTORY ${argRootDir})
set(old_text "argConfigStartMenuDir")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${installscript}) -replace '${old_text}', '${new_text}' | Set-Content ${installscript}"
  WORKING_DIRECTORY ${argRootDir})
set(old_text "argProcessName")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${installscript}) -replace '${old_text}', '${new_text}' | Set-Content ${installscript}"
  WORKING_DIRECTORY ${argRootDir})
set(old_text "argLinkName")
set(new_text "${argTarget}")
execute_process(
  COMMAND
    powershell -Command
    "(Get-Content ${installscript}) -replace '${old_text}', '${new_text}' | Set-Content ${installscript}"
  WORKING_DIRECTORY ${argRootDir})
# --------------------------------------------------------------------------------------------------
if(WIN32)
  set(output_file "${argTarget}-${argVersion}-installer.exe")
elseif(UNIX AND NOT APPLE)
  set(output_file "${argTarget}-${argVersion}-installer.run")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E rename "all" "${argTarget}"
                WORKING_DIRECTORY ${argRootDir}/packages)
execute_process(COMMAND ${argBinarycreator} -c config/config.xml -p packages ${output_file}
                        COMMAND_ECHO STDOUT WORKING_DIRECTORY ${argRootDir})
