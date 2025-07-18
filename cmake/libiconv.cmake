# https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.18.tar.gz
# https://github.com/kiyolee/libiconv-win-build/archive/refs/tags/v1.17-p1.zip devenv libiconv.sln
# /Build

if(MINGW)
  message(STATUS "Using MinGW, skipping libiconv download and extraction")
  return()
endif()

if(MSVC)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.30")
    message(STATUS "Using MSVC 2022 or later, skipping libiconv download and extraction")
    return()
  endif()
endif()

if(ANDROID
   OR IOS
   OR MACOS)
  message(STATUS "Using Android or iOS, macOS, skipping libiconv download and extraction")
  return()
endif()

if(WIN32)
  set(file_name "v1.17-p1")
  set(file_suffix "zip")
  set(file_url
      "https://github.com/kiyolee/libiconv-win-build/archive/refs/tags/${file_name}.${file_suffix}")
else()
  set(file_name "libiconv-1.18")
  set(file_suffix "tar.gz")
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

# Build libiconv if it does not exist
if(WIN32)
  set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-VS2022)
  if(NOT EXISTS ${working_dir}/x64/Release/libiconv.lib)
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
    set(devenv ${COMPILER_PATH}/../../../../../../../Common7/IDE/devenv.exe)
    message(STATUS "[libiconv] ${devenv}")
    message(STATUS "[libiconv] Building libiconv using Visual Studio")
    execute_process(COMMAND ${devenv} libiconv.sln /Build "Release"
                    WORKING_DIRECTORY ${working_dir})
  endif()

  set(X_ICONV
      ON
      CACHE BOOL "Enable iconv option" FORCE)
  set(X_ICONV "libiconv-static")
  add_compile_definitions(X_ICONV)
  include_directories(${CMAKE_SOURCE_DIR}/3rd/${file_name}/include)
  link_directories(${working_dir}/x64/Release)
  message(STATUS "[libiconv] ${working_dir}/x64/Release")
  return()
endif()

set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name})
if(NOT EXISTS ${working_dir}/out/lib/libiconv.so)
  execute_process(COMMAND ./configure --prefix=${working_dir}/out WORKING_DIRECTORY ${working_dir})
  execute_process(COMMAND make WORKING_DIRECTORY ${working_dir})
  execute_process(COMMAND make install WORKING_DIRECTORY ${working_dir})
endif()

if(EXISTS ${working_dir}/out/lib/libiconv.so)
  # cmake-format: off
  set(X_ICONV ON CACHE BOOL "Enable iconv option" FORCE)
  set(X_ICONV "iconv" "charset")
  add_compile_definitions(X_ICONV)
  include_directories(${working_dir}/out/include)
  link_directories(${working_dir}/out/lib)
  # cmake-format: on
endif()
