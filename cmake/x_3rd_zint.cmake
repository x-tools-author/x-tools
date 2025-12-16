# https://github.com/zint/zint
set(package_version "2.15.0")
set(file_name "zint-${package_version}")
set(file_url "https://codeload.github.com/zint/zint/zip/refs/tags/${package_version}")
# cmake-format: off
set(ZINT_SHARED OFF CACHE BOOL "Shared Zint" FORCE)
set(ZINT_STATIC ON CACHE BOOL "Static Zint" FORCE)
set(ZINT_USE_QT ON CACHE BOOL "Use Qt" FORCE)
find_package(PNG)
if(PNG_FOUND)
  set(ZINT_USE_PNG ON CACHE BOOL "Use PNG" FORCE)
else()
  set(ZINT_USE_PNG OFF CACHE BOOL "Use PNG" FORCE)
endif()
set(ZINT_FRONTEND OFF CACHE BOOL "Build Zint frontend" FORCE)
if(QT_VERSION_MAJOR EQUAL 6)
  set(ZINT_QT6 ON CACHE BOOL "Use Qt6" FORCE)
endif()
# cmake-format: on

macro(x_remove_zint_files)
  file(GLOB_RECURSE ZINT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/barcode/*.*")
  foreach(file ${ZINT_FILES})
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[Zint]Remove file: ${file}")
  endforeach()
endmacro()

# --------------------------------------------------------------------------------------------------
if(ANDROID OR IOS)
  x_remove_zint_files()
  return()
endif()

if(QT_VERSION VERSION_LESS "5.11.0")
  x_remove_zint_files()
  return()
endif()

if(APPLE)
  if(QT_VERSION VERSION_LESS "6.2.0")
    x_remove_zint_files()
    return()
  endif()
endif()

if(MINGW)
  x_remove_zint_files()
  return()
endif()

# --------------------------------------------------------------------------------------------------
# Download and unzip the Zint repository if it does not exist
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rd/${file_name}.zip")
  message(STATUS "[zint] Downloading Zint repository from ${file_url}...")
  file(
    DOWNLOAD "${file_url}" "${CMAKE_SOURCE_DIR}/3rd/${file_name}.zip"
    SHOW_PROGRESS
    STATUS download_status)
  if(NOT download_status EQUAL 0)
    message(FATAL_ERROR "[zint] Failed to download Zint repository: ${download_status}")
    x_remove_zint_files()
    return()
  endif()
endif()

# Unzip the Zint repository if it does not exist
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/3rd/${file_name}/CMakeLists.txt")
  message(STATUS "[zint] Unzipping Zint repository...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${file_name}.zip"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/3rd")
endif()

if(EXISTS "${X_LIBS_DIR}/${file_name}/include/zint.h" AND (WIN32 OR APPLE))
  include_directories(${X_LIBS_DIR}/${file_name}/include)
  link_directories(${X_LIBS_DIR}/${file_name}/lib)
  set(CMAKE_PREFIX_PATH ${X_LIBS_DIR}/${file_name} ${CMAKE_PREFIX_PATH})
  find_package(zint REQUIRED)
  if(NOT zint_FOUND)
    message(FATAL_ERROR "[zint] Failed to find Zint in ${X_LIBS_DIR}/${file_name}")
    return()
  endif()

  if(WIN32)
    set(X_ZINT_LIBS zint::zint-static)
  elseif(APPLE)
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${X_LIBS_DIR}/${file_name}/lib/libzint.a
              ${X_LIBS_DIR}/${file_name}/lib/libzint-static.a)
    set(X_ZINT_LIBS zint)
  endif()
else()
  # Add the Zint subdirectory to the project
  include_directories(${CMAKE_SOURCE_DIR}/3rd/${file_name})
  add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${file_name})

  # Specify the destination directory for Zint
  set_property(TARGET QZint PROPERTY FOLDER "3rd")
  set_property(TARGET uninstall PROPERTY FOLDER "3rd")
  if(WIN32)
    set_property(TARGET zint_bundled_getopt PROPERTY FOLDER "3rd")
  endif()
  set_property(TARGET zint-qt PROPERTY FOLDER "3rd")
  set_property(TARGET zint-static PROPERTY FOLDER "3rd")

  if(QT_VERSION VERSION_GREATER_EQUAL "6.8.0")
    # Auto install Zint
    add_custom_command(
      OUTPUT ${X_LIBS_DIR}/${file_name}/install.stamp
      COMMAND ${CMAKE_COMMAND} --install . --prefix ${X_LIBS_DIR}/${file_name}
      COMMAND ${CMAKE_COMMAND} -E touch ${X_LIBS_DIR}/${file_name}/install.stamp
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${file_name}
      COMMENT "[zint] Installing Zint to ${X_LIBS_DIR}/${file_name}")
    add_custom_target(QZint_auto_install ALL DEPENDS ${X_LIBS_DIR}/${file_name}/install.stamp)
    add_dependencies(QZint_auto_install QZint zint-qt zint-static)
    set_property(TARGET QZint_auto_install PROPERTY FOLDER "3rd")
  endif()

  set(X_ZINT_LIBS zint-static)
  add_compile_definitions(X_ZINT_USING_SRC)
endif()

set(X_ENABLE_ZINT ON)
list(APPEND X_ZINT_LIBS QZint)
add_compile_definitions(X_ENABLE_ZINT)
list(APPEND X_LIBS ${X_ZINT_LIBS})
