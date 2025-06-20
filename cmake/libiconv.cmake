# https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.18.tar.gz
# https://github.com/kiyolee/libiconv-win-build/archive/refs/tags/v1.17-p1.zip devenv libiconv.sln
# /Build

if(MINGW)
  message(STATUS "Using MinGW, skipping libiconv download and extraction")
  return()
endif()

if(WIN32)
  set(file_name "v1.17-p1")
  set(file_suffix "zip")
  set(file_url
      "https://github.com/kiyolee/libiconv-win-build/archive/refs/tags/${file_name}.${file_suffix}")
else()
  set(file_name "libiconv-1.18")
  set(file_suffix ".tar.gz")
  set(file_url "https://ftp.gnu.org/pub/gnu/libiconv/${file_name}.${file_suffix}")
endif()

# Download libiconv if it does not exist
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${file_name}.${file_suffix})
  message(STATUS "Downloading ${file_name} from ${file_url}")
  file(
    DOWNLOAD ${file_url} ${CMAKE_SOURCE_DIR}/3rd/${file_name}.${file_suffix}
    STATUS download_status
    SHOW_PROGRESS)
  if(NOT download_status EQUAL 0)
    message(FATAL_ERROR "Failed to download ${file_name}: ${download_status}")
  endif()
endif()

# Extract libiconv if it does not exist
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${file_name})
  message(STATUS "Extracting ${file_name}.${file_suffix}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${file_name}.${file_suffix}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  if(WIN32)
    set(tmp_dir libiconv-win-build-1.17-p1)
    execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${tmp_dir} ${file_name}
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  endif()
endif()

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-VS2022/x64/Release/libiconv.lib)
  # Build libiconv if it does not exist
endif()
