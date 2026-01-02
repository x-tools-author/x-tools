# * argTarget: cmake target
# * argPacketName: packet name of deb packet
# * argFriendlyName: desktop entry file name
# * argVersion: the application version
# * argWorkingDir: working dir
# * argLowerTargetName: lower case of argTarget, such as "xTools" -> "xtools"
# * argTool: linuxdeployqt tool path
# * argSrcDir: project source dir
# * DargPackageType: deb or appimage
# * DargTargetFile: target file path

message(STATUS "[xTools.Linux] argTarget: ${argTarget}")
message(STATUS "[xTools.Linux] argPacketName: ${argPacketName}")
message(STATUS "[xTools.Linux] argFriendlyName: ${argFriendlyName}")
message(STATUS "[xTools.Linux] argVersion: ${argVersion}")
message(STATUS "[xTools.Linux] argWorkingDir: ${argWorkingDir}")
message(STATUS "[xTools.Linux] argLowerTargetName: ${argLowerTargetName}")
message(STATUS "[xTools.Linux] argTool: ${argTool}")
message(STATUS "[xTools.Linux] argSrcDir: ${argSrcDir}")
message(STATUS "[xTools.Linux] argQmakePath: ${argQmakePath}")
message(STATUS "[xTools.Linux] argTargetFile: ${argTargetFile}")
message(STATUS "[xTools.Linux] argPackageType: ${argPackageType}")
message(STATUS "[xTools.Linux] argAssetName: ${argAssetName}")

if(NOT EXISTS ${argTool})
  message(FATAL_ERROR "[xTools.Linux] linuxdeployqt tool not found: ${argTool}")
endif()

# Remove old working dir then create a new one
set(AppImageRootDir ${argWorkingDir}/appimage)
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${argWorkingDir} -rf ||
                        ${CMAKE_COMMAND} -E true)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                        ${argSrcDir}/cmake/linux/app ${AppImageRootDir})
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${argTargetFile}
                        ${AppImageRootDir}/usr/bin/${argPacketName})

# Update control file
set(control_file ${AppImageRootDir}/DEBIAN/control)
set(old_text argPacketName)
set(new_text ${argPacketName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${control_file})

set(old_text argVersion)
set(new_text ${argVersion})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${control_file})

set(desktop_file_name_tmp ${AppImageRootDir}/usr/share/applications/app.desktop)
set(desktop_file_name ${AppImageRootDir}/usr/share/applications/${argPacketName}.desktop)
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

# Rename the icon
set(old_text ${AppImageRootDir}/usr/share/icons/icon.png)
set(new_text ${AppImageRootDir}/usr/share/icons/${argPacketName}.png)
execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${old_text} ${new_text})

set(old_text icon.png)
set(new_text ${argPacketName})
execute_process(COMMAND sed -i s/${old_text}/${new_text}/g ${desktop_file_name})

# Get directory from argTargetFile
get_filename_component(target_dir ${argTargetFile} DIRECTORY)

# Copy files to bin
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${target_dir}/translations"
                        ${AppImageRootDir}/usr/bin/translations)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${target_dir}/scripts"
                        ${AppImageRootDir}/usr/bin/scripts)

# Make deb directory from appimage
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory appimage deb
                WORKING_DIRECTORY ${argWorkingDir})

# Make AppImage
message(STATUS "Create AppImage package")
execute_process(
  COMMAND
    ${CMAKE_COMMAND} -E env VERSION=v${argVersion} ${argTool}
    usr/share/applications/${argPacketName}.desktop -always-overwrite -bundle-non-qt-libs
    -qmake=${argQmakePath} -appimage
  WORKING_DIRECTORY ${AppImageRootDir})

# Rename the AppImage
file(GLOB appimages ${AppImageRootDir}/*.AppImage)
foreach(appimage ${appimages})
  message(STATUS "Rename AppImage ${appimage} to ${argAssetName}.AppImage")
  execute_process(COMMAND mv ${appimage} ${argAssetName}.AppImage
                  WORKING_DIRECTORY ${AppImageRootDir})
endforeach()

# Make deb
message(STATUS "Create deb package ${argAssetName}.deb")
if(FALSE)
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${argAssetName}.AppImage
                          ../deb/usr/bin/${argPacketName} WORKING_DIRECTORY ${AppImageRootDir})
else()
  execute_process(
    COMMAND
      ${CMAKE_COMMAND} -E env VERSION=v${argVersion} ${argTool}
      usr/share/applications/${argPacketName}.desktop -always-overwrite -bundle-non-qt-libs
      -qmake=${argQmakePath}
    WORKING_DIRECTORY ${argWorkingDir}/deb)
  execute_process(COMMAND rm ./.DirIcon
                  COMMAND rm ./AppRun
                  COMMAND rm ./${argPacketName}.desktop
                  COMMAND rm ./${argPacketName}.png
                  WORKING_DIRECTORY ${argWorkingDir}/deb)
  execute_process(COMMAND mkdir opt
                  COMMAND mv usr opt/${argFriendlyName} WORKING_DIRECTORY ${argWorkingDir}/deb)
  execute_process(COMMAND mkdir usr 
                  COMMAND ${CMAKE_COMMAND} -E copy_directory opt/${argFriendlyName}/share usr/share
                  WORKING_DIRECTORY ${argWorkingDir}/deb)
  execute_process(COMMAND mv opt/${argFriendlyName}/translations opt/${argFriendlyName}/bin/translations
                  WORKING_DIRECTORY ${argWorkingDir}/deb)
  execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory usr/share/doc
                  WORKING_DIRECTORY ${argWorkingDir}/deb)
  set(desktop_file_name ${argWorkingDir}/deb/usr/share/applications/${argPacketName}.desktop)
  set(old_text "Exec=${argPacketName}")
  set(new_text "Exec=/opt/${argFriendlyName}/bin/${argPacketName}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E echo "sed -i s/${old_text}/${new_text}/g ${desktop_file_name}")
  execute_process(COMMAND sed -i s|${old_text}|${new_text}|g ${desktop_file_name})
endif()
execute_process(COMMAND dpkg -b ./ ${argAssetName}.deb WORKING_DIRECTORY ${argWorkingDir}/deb)
