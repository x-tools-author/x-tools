# https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.18.tar.gz
# https://github.com/kiyolee/libiconv-win-build/archive/refs/tags/v1.17-p1.zip devenv libiconv.sln
# /Build

if(MINGW)
  message(STATUS "[xTools-iconv] Using MinGW, skipping libiconv download and extraction")
  return()
endif()

if(MSVC)
  if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.30")
    set(X_VS "VS2022")
  elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.20")
    set(X_VS "VS2019")
  elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.10")
    set(X_VS "VS2017")
  elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.00")
    set(X_VS "VS2015")
  else()
    set(X_VS "VS2013")
  endif()
endif()

if(ANDROID
   OR IOS
   OR MACOS)
  message(
    STATUS "[xTools-iconv] Using Android or iOS, macOS, skipping libiconv download and extraction")
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
  message(STATUS "[xTools-iconv] Downloading ${file_name} from ${file_url}")
  file(
    DOWNLOAD ${file_url} ${CMAKE_SOURCE_DIR}/3rd/${file_name}.${file_suffix}
    STATUS download_status
    SHOW_PROGRESS)
  if(NOT download_status EQUAL 0)
    message(FATAL_ERROR "[xTools-iconv] Failed to download ${file_name}: ${download_status}")
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
  set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-${X_VS})
  message(STATUS "[xTools-iconv] Working directory: ${working_dir}")

  set(lib_dir ${working_dir}/x64/Release)
  message(STATUS "[xTools-iconv] Library directory: ${lib_dir}")
  if(NOT EXISTS ${lib_dir}/libiconv.lib)
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
    set(devenv ${COMPILER_PATH}/../../../../../../../Common7/IDE/devenv.exe)
    message(STATUS "[xTools-iconv] ${devenv}")
    message(STATUS "[xTools-iconv] Building libiconv using Visual Studio")
    execute_process(COMMAND ${devenv} libiconv.sln /Build "Release|x64"
                    WORKING_DIRECTORY ${working_dir})
  endif()

  add_compile_definitions(X_ENABLE_ICONV)
  include_directories(${CMAKE_SOURCE_DIR}/3rd/${file_name}/include)
  link_directories(${lib_dir})
  list(APPEND X_LIBS "libiconv-static")
  set(X_ICONV_LIBS "libiconv-static")
elseif(APPLE)
  set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name})
  if(NOT EXISTS ${working_dir}/out/lib/libiconv.la)
    execute_process(COMMAND ./configure --prefix=${working_dir}/out
                    WORKING_DIRECTORY ${working_dir})
    execute_process(COMMAND make WORKING_DIRECTORY ${working_dir})
    execute_process(COMMAND make install WORKING_DIRECTORY ${working_dir})
  endif()

  if(EXISTS ${working_dir}/out/lib/libiconv.la)
    add_compile_definitions(X_ENABLE_ICONV)
    include_directories(${working_dir}/out/include)
    link_directories(${working_dir}/out/lib)
    list(APPEND X_LIBS ${working_dir}/out/lib/libiconv.la)
  endif()
else()
  set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name})
  if(NOT EXISTS ${working_dir}/out/lib/libiconv.so)
    execute_process(COMMAND ./configure --prefix=${working_dir}/out
                    WORKING_DIRECTORY ${working_dir})
    execute_process(COMMAND make WORKING_DIRECTORY ${working_dir})
    execute_process(COMMAND make install WORKING_DIRECTORY ${working_dir})
  endif()

  if(EXISTS ${working_dir}/out/lib/libiconv.so)
    add_compile_definitions(X_ENABLE_ICONV)
    include_directories(${working_dir}/out/include)
    link_directories(${working_dir}/out/lib)
    list(APPEND X_LIBS "iconv")
  endif()
endif()
