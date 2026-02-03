# * https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.18.tar.gz
# * https://github.com/kiyolee/libiconv-win-build/archive/refs/tags/v1.17-p1.zip
# * devenv libiconv.sln /Build "Release|x64"

# --------------------------------------------------------------------------------------------------
if(MINGW)
  message(STATUS "[xTools-iconv] Using MinGW, skipping libiconv download and extraction")
  add_compile_definitions(X_ENABLE_ICONV=0)
  return()
endif()

if(LINUX OR ANDROID)
  add_compile_definitions(X_ENABLE_ICONV=0)
  return()
endif()

# --------------------------------------------------------------------------------------------------
if(WIN32)
  set(iconv_version "1.18-p1")
  set(file_name "v${iconv_version}")
  set(file_suffix "zip")
  set(file_url "https://github.com/kiyolee/libiconv-win-build/archive/refs/tags")
  set(file_url "${file_url}/${file_name}.${file_suffix}")
else()
  set(file_name "libiconv-1.18")
  set(file_suffix "tar.gz")
  set(file_url "https://ftp.gnu.org/pub/gnu/libiconv/${file_name}.${file_suffix}")
endif()

# --------------------------------------------------------------------------------------------------
# Download libiconv if it does not exist
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${file_name}.${file_suffix})
  message(STATUS "[xTools-iconv] Downloading ${file_name} from ${file_url}")
  file(
    DOWNLOAD ${file_url} ${CMAKE_SOURCE_DIR}/3rd/${file_name}.${file_suffix}
    STATUS download_status
    SHOW_PROGRESS)
  if(NOT download_status EQUAL 0)
    # Remove libiconv zip file
    file(REMOVE ${CMAKE_SOURCE_DIR}/3rd/${file_name}.${file_suffix})
    add_compile_definitions(X_ENABLE_ICONV=0)
    return()
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Extract libiconv if it does not exist
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${file_name})
  message(STATUS "Extracting ${file_name}.${file_suffix}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${file_name}.${file_suffix}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  if(WIN32)
    set(tmp_dir libiconv-win-build-${iconv_version})
    execute_process(COMMAND ${CMAKE_COMMAND} -E rename ${tmp_dir} ${file_name}
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Build libiconv if it does not exist
if(WIN32)
  if(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.50")
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-VS2026)
      set(src_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-VS2022)
      set(dst_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-VS2026)
      if(NOT EXISTS ${dst_dir})
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${src_dir} ${dst_dir})
        file(GLOB_RECURSE pro_files "${dst_dir}/*.vcxproj")
        foreach(pro_file IN LISTS pro_files)
          # Replace all occurrences of "v143" with "v145"
          set(old_text "v143")
          set(new_text "v145")
          execute_process(COMMAND powershell -Command "(Get-Content ${pro_file}) -replace
        '${old_text}', '${new_text}' | Set-Content ${pro_file}")
        endforeach()
      endif()
    endif()

    set(X_VS "VS2026")
  elseif(NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.30")
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
  set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name}/build-${X_VS})
  if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "ARM64")
    set(working_dir ${working_dir}-arm64)
  endif()

  message(STATUS "[xTools-iconv] Working directory: ${working_dir}")

  set(lib_dir ${working_dir}/x64/Release)
  message(STATUS "[xTools-iconv] Library directory: ${lib_dir}")
  if(NOT EXISTS ${lib_dir}/libiconv.lib)
    cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH COMPILER_PATH)
    set(devenv ${COMPILER_PATH}/../../../../../../../Common7/IDE/devenv.exe)
    message(STATUS "[xTools-iconv] ${devenv}")
    message(STATUS "[xTools-iconv] Building libiconv using Visual Studio")
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "ARM64")
      execute_process(COMMAND ${devenv} libiconv.sln /Build "Release|ARM64"
                      WORKING_DIRECTORY ${working_dir})
    else()
      execute_process(COMMAND ${devenv} libiconv.sln /Build "Release|x64"
                      WORKING_DIRECTORY ${working_dir})
    endif()
  endif()

  if(EXISTS ${lib_dir}/libiconv.lib)
    include_directories(${CMAKE_SOURCE_DIR}/3rd/${file_name}/include)
    link_directories(${lib_dir})
    list(APPEND X_LIBS libiconv-static)
    add_compile_definitions(X_ENABLE_ICONV=1)
  else()
    add_compile_definitions(X_ENABLE_ICONV=0)
  endif()
elseif(APPLE)
  list(APPEND X_LIBS iconv)
  add_compile_definitions(X_ENABLE_ICONV=1)
else()
  set(working_dir ${CMAKE_SOURCE_DIR}/3rd/${file_name})
  if(NOT EXISTS ${working_dir}/out/lib/libiconv.so)
    execute_process(COMMAND ./configure --prefix=${working_dir}/out
                    WORKING_DIRECTORY ${working_dir})
    execute_process(COMMAND make WORKING_DIRECTORY ${working_dir})
    execute_process(COMMAND make install WORKING_DIRECTORY ${working_dir})
  endif()

  if(EXISTS ${working_dir}/out/lib/libiconv.so)
    include_directories(${working_dir}/out/include)
    link_directories(${working_dir}/out/lib)
    list(APPEND X_LIBS iconv)
    add_compile_definitions(X_ENABLE_ICONV=1)
  else()
    add_compile_definitions(X_ENABLE_ICONV=0)
  endif()
endif()
