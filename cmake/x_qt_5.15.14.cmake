# https://master.dl.sourceforge.net/project/fsu0413-qtbuilds/Qt5.15/Windows-x86/MinGW-GCC/Qt5.15.14-Windows-x86-MinGW8.1.0-20240527.7z?viasf=1&fid=b5089b8baf0ecaaf
set(qt_5_15_14 "Qt5.15.14")
set(qt_5_15_14_url "https://master.dl.sourceforge.net/project/fsu0413-qtbuilds")
set(qt_5_15_14_url "${qt_5_15_14_url}/Qt5.15/Windows-x86/MinGW-GCC")
set(qt_5_15_14_url "${qt_5_15_14_url}/Qt5.15.14-Windows-x86-MinGW8.1.0-20240527.7z")
set(qt_5_15_14_url "${qt_5_15_14_url}?viasf=1&fid=b5089b8baf0ecaaf")
set(dst_path "${CMAKE_SOURCE_DIR}/tools/${qt_5_15_14}.7z")

# --------------------------------------------------------------------------------------------------
# Download from 3rd builder if it is not existed...
if(NOT EXISTS ${dst_path} AND NOT EXISTS "${CMAKE_SOURCE_DIR}/tools/${qt_5_15_14}")
  # Download Qt 5.15.14
  message(STATUS "[xTools.Win7] Downloading Qt 5.15.14...")
  file(
    DOWNLOAD ${qt_5_15_14_url} ${dst_path}
    SHOW_PROGRESS
    STATUS download_status)
  list(GET download_status 0 download_result)
  if(NOT download_result EQUAL 0)
    file(REMOVE ${dst_path})
    message(FATAL_ERROR "[xTools.Win7] Failed to download Qt 5.15.14 from ${qt_5_15_14_url}")
  endif()

  # Extract Qt 5.15.14
  message(STATUS "[xTools.Win7] Extracting Qt 5.15.14...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${dst_path}
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tools/")
endif()

# --------------------------------------------------------------------------------------------------
# Extract from 3rd builder if it is not existed...
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tools/${qt_5_15_14}")
  if(EXISTS ${dst_path})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${dst_path}
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tools/")
    # Rename extracted folder to expected name if needed
    set(extracted_folder "${CMAKE_SOURCE_DIR}/tools/Qt5.15.14-Windows-x86-MinGW8.1.0")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E rename ${extracted_folder}
              "${CMAKE_SOURCE_DIR}/tools/${qt_5_15_14}"
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tools/")
  endif()
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tools/${qt_5_15_14}")
  message(FATAL_ERROR "[xTools.Win7] Qt 5.15.14 setup failed.")
endif()

# --------------------------------------------------------------------------------------------------
# Download MinGW if it is not existed...
# https://mirrors.20i.com/pub/qt.io/online/qtsdkrepository/windows_x86/desktop/tools_mingw81/qt.tools.win32_mingw810/8.1.0-1-202411201005i686-8.1.0-gdb-11.2.0-release-posix-dwarf-rt_v6-rev0.7z
set(mingw_url "https://mirrors.20i.com/pub/qt.io/online/qtsdkrepository/windows_x86/desktop")
set(mingw_url "${mingw_url}/tools_mingw81/qt.tools.win32_mingw810")
set(mingw_url
    "${mingw_url}/8.1.0-1-202411201005i686-8.1.0-gdb-11.2.0-release-posix-dwarf-rt_v6-rev0.7z")
set(mingw_dst_path "${CMAKE_SOURCE_DIR}/tools/MinGW810_32.7z")
if(NOT EXISTS ${mingw_dst_path} AND NOT EXISTS "${CMAKE_SOURCE_DIR}/tools/MinGW810_32")
  message(STATUS "[xTools.Win7] Downloading MinGW...")
  file(
    DOWNLOAD ${mingw_url} ${mingw_dst_path}
    SHOW_PROGRESS
    STATUS download_status)
  list(GET download_status 0 download_result)
  if(NOT download_result EQUAL 0)
    file(REMOVE ${mingw_dst_path})
    message(FATAL_ERROR "[xTools.Win7] Failed to download MinGW from ${mingw_url}")
  endif()

  message(STATUS "[xTools.Win7] Extracting MinGW...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${mingw_dst_path}
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tools/")
endif()

if(EXISTS ${mingw_dst_path} AND NOT EXISTS "${CMAKE_SOURCE_DIR}/tools/MinGW810_32")
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
                          "${CMAKE_SOURCE_DIR}/tools/MinGW810_32")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${mingw_dst_path}
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tools/MinGW810_32")
endif()

# --------------------------------------------------------------------------------------------------
# Extract MinGW if it is not existed...
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tools/MinGW810_32")
  if(EXISTS ${mingw_dst_path})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${mingw_dst_path}
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/tools/")
  endif()
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tools/MinGW810_32")
  message(FATAL_ERROR "[xTools.Win7] MinGW setup failed.")
endif()
