# Setup environment for building with Qt 5.6.3(mingw 4.9.2 32bits)

# Download Qt 5.6.3 from:
# https://master.dl.sourceforge.net/project/fsu0413-qtbuilds/Qt5.6/Windows-x86/Qt5.6.3-Windows-x86-MinGW4.9.4-20200104.7z?viasf=1
set(file_name "Qt5.6.3-Windows-x86-MinGW4.9.4-20200104")
set(download_url "https://master.dl.sourceforge.net/project/fsu0413-qtbuilds/Qt5.6")
set(src "${download_url}/Windows-x86/${file_name}.7z?viasf=1")
set(dst "${CMAKE_CURRENT_SOURCE_DIR}/tools/${file_name}.7z")
if(NOT EXISTS ${dst} AND NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/${file_name}")
  message(STATUS "[xTools.winXP] Downloading Qt 5.6.3...")
  file(
    DOWNLOAD "${src}" "${dst}"
    SHOW_PROGRESS
    STATUS download_status)
  list(GET download_status 0 download_result)
  if(NOT download_result EQUAL 0)
    message(FATAL_ERROR "[xTools.winXP] Failed to download Qt 5.6.3 from ${src}")
    file(REMOVE "${dst}")
    return()
  endif()

  message(STATUS "[xTools.winXP] Extracting Qt 5.6.3...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvf "${dst}"
                  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tools")
  execute_process(COMMAND QQtPatcher.exe
                  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tools/${file_name}")
  message(STATUS "[xTools.winXP] Qt 5.6.3 is ready.")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/${file_name}")
  if(EXISTS "${dst}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvf "${dst}"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tools")
  endif()
endif()

# Download mingw 4.9.2 from:
# https://download.qt.io/online/qtsdkrepository/windows_x86/desktop/tools_mingw/qt.tools.win32_mingw492/4.9.2-1i686-4.9.2-release-posix-dwarf-rt_v3-rev1.7z
set(mingw_file_name "4.9.2-1i686-4.9.2-release-posix-dwarf-rt_v3-rev1")
set(mingw_download_url "https://download.qt.io/online/qtsdkrepository/windows_x86/desktop")
set(mingw_download_url "${mingw_download_url}//tools_mingw/qt.tools.win32_mingw492")
set(mingw_src "${mingw_download_url}/${mingw_file_name}.7z")
set(mingw_dst "${CMAKE_CURRENT_SOURCE_DIR}/tools/${mingw_file_name}.7z")
if(NOT EXISTS ${mingw_dst} AND NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/Tools")
  message(STATUS "[xTools.winXP] Downloading mingw 4.9.2...")
  file(
    DOWNLOAD "${mingw_src}" "${mingw_dst}"
    SHOW_PROGRESS
    STATUS download_status)
  list(GET download_status 0 download_result)
  if(NOT download_result EQUAL 0)
    message(FATAL_ERROR "[xTools.winXP] Failed to download mingw 4.9.2 from ${mingw_src}")
    file(REMOVE "${mingw_dst}")
    return()
  endif()

  message(STATUS "[xTools.winXP] Extracting mingw 4.9.2...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvf "${mingw_dst}"
                  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tools")
  message(STATUS "[xTools.winXP] mingw 4.9.2 is ready.")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tools/Tools")
  if(EXISTS "${mingw_dst}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvf "${mingw_dst}"
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tools")
  endif()
endif()
