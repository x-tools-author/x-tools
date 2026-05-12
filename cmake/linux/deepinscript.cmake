# * argTarget: cmake target
# * argPacketName: packet name of deb packet
# * argFriendlyName: desktop entry file name
# * argVersion: the application version
# * argAppID: app id for deepin, such as "com.deepin.demo"
# * argWorkingDir: working dir
# * argLowerTargetName: lower case of argTarget, such as "xTools" -> "xtools"
# * argTool: linuxdeployqt tool path
# * argSrcDir: project source dir
# * argTargetFile: target file path

message(STATUS "[xTools.Deepin] argTarget: ${argTarget}")
message(STATUS "[xTools.Deepin] argPacketName: ${argPacketName}")
message(STATUS "[xTools.Deepin] argFriendlyName: ${argFriendlyName}")
message(STATUS "[xTools.Deepin] argVersion: ${argVersion}")
message(STATUS "[xTools.Deepin] argAppID: ${argAppID}")
message(STATUS "[xTools.Deepin] argWorkingDir: ${argWorkingDir}")
message(STATUS "[xTools.Deepin] argLowerTargetName: ${argLowerTargetName}")
message(STATUS "[xTools.Deepin] argTool: ${argTool}")
message(STATUS "[xTools.Deepin] argSrcDir: ${argSrcDir}")
message(STATUS "[xTools.Deepin] argQmakePath: ${argQmakePath}")
message(STATUS "[xTools.Deepin] argTargetFile: ${argTargetFile}")
message(STATUS "[xTools.Deepin] argAssetName: ${argAssetName}")

if(NOT EXISTS ${argTool})
  # Using system linuxdeployqt
  set(argTool linuxdeployqt)
endif()

# Remove old working dir then create a new one
set(DebRootDir ${argWorkingDir}/deb)
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${argWorkingDir})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                        ${argSrcDir}/cmake/linux/deepin ${DebRootDir})

message(STATUS "[xTools.Deepin] Rename apps to ${argAppID}")
execute_process(COMMAND ${CMAKE_COMMAND} -E rename appid ${argAppID}
                WORKING_DIRECTORY ${DebRootDir}/opt/apps)

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${argTargetFile}
                        ${DebRootDir}/opt/apps/${argAppID}/files/${argPacketName})

# Update control file
set(control_file ${DebRootDir}/DEBIAN/control)
set(old_text argPacketName)
set(new_text ${argPacketName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${control_file})

set(old_text argVersion)
set(new_text ${argVersion})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${control_file})
if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "aarch64")
  set(old_text amd64)
  set(new_text arm64)
  execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${control_file})
endif()

# Update desktop file
set(desktop_file_name_tmp
    ${DebRootDir}/opt/apps/${argAppID}/entries/applications/com.deepin.demo.desktop)
set(desktop_file_name ${DebRootDir}/opt/apps/${argAppID}/entries/applications/${argAppID}.desktop)
execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${desktop_file_name_tmp} ${desktop_file_name})

set(old_text argFriendlyName)
set(new_text ${argFriendlyName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})
set(old_text argPacketName)
set(new_text ${argPacketName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})
set(old_text argTarget)
set(new_text ${argTarget})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})
set(old_text Icon=icon.svg)
set(new_text Icon=${argAppID}.svg)
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})

# Rename the icon
set(icon_path ${DebRootDir}/opt/apps/${argAppID}/entries/icons/hicolor/scalable/apps)
set(old_icon_path ${icon_path}/com.deepin.demo.svg)
set(new_icon_path ${icon_path}/${argAppID}.svg)
execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${old_icon_path} ${new_icon_path})

set(old_text com.deepin.demo)
set(new_text ${argAppID})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})

# Get directory from argTargetFile
get_filename_component(target_dir ${argTargetFile} DIRECTORY)

# Copy files to deepin package
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${target_dir}/translations"
                        ${DebRootDir}/opt/apps/${argAppID}/files/translations)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${target_dir}/scripts"
                        ${DebRootDir}/opt/apps/${argAppID}/files/scripts)

# Update desktop Exec for linuxdeployqt discovery (must be resolvable in build workspace)
set(old_text "Exec=${argPacketName}")
set(new_text "Exec=opt/apps/${argAppID}/files/${argPacketName}")
execute_process(COMMAND sed -i s|${old_text}|${new_text}|g ${desktop_file_name})

# For Deepin package, all operations are performed directly in deb working directory
message(STATUS "Create deb package ${argAssetName}.deb")

# Build the deb package
execute_process(
  COMMAND
    ${CMAKE_COMMAND} -E env VERSION=v${argVersion} ${argTool}
    opt/apps/${argAppID}/entries/applications/${argAppID}.desktop -always-overwrite
    -bundle-non-qt-libs -qmake=${argQmakePath}
  WORKING_DIRECTORY ${DebRootDir})

# Restore runtime Exec path expected by Deepin package
set(old_text "Exec=opt/apps/${argAppID}/files/${argPacketName}")
set(new_text "Exec=/opt/apps/${argAppID}/files/${argPacketName}")
execute_process(COMMAND sed -i s|${old_text}|${new_text}|g ${desktop_file_name})

execute_process(COMMAND dpkg -b ./ ${argAssetName}.deb WORKING_DIRECTORY ${DebRootDir})
