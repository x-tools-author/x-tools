include(${CMAKE_SOURCE_DIR}/cmake/x_qt_deploy.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_linguist.cmake)

include(${CMAKE_SOURCE_DIR}/cmake/x_qt_charts.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_serialbus.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_serialport.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_bluetooth.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_websockets.cmake)

macro(x_remove_all_files_of_qt_optional module_name)
  message(STATUS "[xTools-${module_name}]Removing all flow files...")
  # remove "qt" prefix
  string(SUBSTRING ${module_name} 2 -1 module_name)
  file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/${module_name}*.*")
  foreach(file ${files})
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[xTools-${module_name}]Removed file: ${file}")
  endforeach()
endmacro()

function(x_download_qt_optional module_name)
  # cmake-format: off
  # https://codeload.github.com/qt/qtmqtt/zip/refs/heads/6.9.2
  set(url "https://codeload.github.com/qt/${module_name}/zip/refs/heads/${QT_VERSION}")
  if(EXISTS ${X_3RD_DIR}/${module_name}-${QT_VERSION}.zip)
    set(${module_name}_DOWNLOADED TRUE PARENT_SCOPE)
    message(STATUS "[xTools-${module_name}]${module_name} already downloaded.")
    return()
  endif()

  file(DOWNLOAD ${url} ${X_3RD_DIR}/${module_name}-${QT_VERSION}.zip SHOW_PROGRESS STATUS download_status)
  list(GET download_status 0 download_status_code)
  if(download_status_code EQUAL 0)
    set(${module_name}_DOWNLOADED TRUE PARENT_SCOPE)
  else()
    message(WARNING "[xTools-${module_name}]Download ${module_name} failed: ${download_status}")
    file(REMOVE ${X_3RD_DIR}/${module_name}-${QT_VERSION}.zip)
    x_remove_all_files_of_qt_optional(${module_name})
    set(${module_name}_DOWNLOADED FALSE PARENT_SCOPE)
  endif()
  # cmake-format: on
endfunction()

function(x_extract_qt_optional_module module_name)
  # cmake-format: off
  if(EXISTS ${X_3RD_DIR}/${module_name}-${QT_VERSION})
    set(${module_name}_EXTRACTED TRUE PARENT_SCOPE)
    message(STATUS "[xTools-${module_name}]${module_name} already extracted.")
    return()
  endif()

  message(STATUS "Extracting ${module_name}...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${X_3RD_DIR}/${module_name}-${QT_VERSION}.zip
                  WORKING_DIRECTORY ${X_3RD_DIR}
                  RESULT_VARIABLE extract_result)
  if(extract_result EQUAL 0)
    set(${module_name}_EXTRACTED TRUE PARENT_SCOPE)
  else()
    message(WARNING "Extract ${module_name} failed")
    x_remove_all_files_of_qt_optional(${module_name})
    set(${module_name}_EXTRACTED FALSE PARENT_SCOPE)
    return()
  endif()
  # cmake-format: on
endfunction()

function(x_import_qt_optional_module module_name cmake_package)
  # cmake-format: off
  x_download_qt_optional(${module_name})
  if(NOT ${module_name}_DOWNLOADED)
    return()
  endif()

  x_extract_qt_optional_module(${module_name})
  if(NOT ${module_name}_EXTRACTED)
    return()
  endif()

  set(module_dst_dir ${X_LIBS_DIR}/${module_name}-${QT_VERSION})
  if(EXISTS ${module_dst_dir}/include)
    # Remove "qt" prefix
    set(CMAKE_PREFIX_PATH ${module_dst_dir} ${CMAKE_PREFIX_PATH})
    set(package_name "Qt6${cmake_package}")
    set(lib_name "Qt6::${cmake_package}")
    find_package(${package_name} REQUIRED)
    if(${package_name}_FOUND)
      set(X_LIBS ${X_LIBS} ${lib_name} PARENT_SCOPE)
      set(tmp "X_ENABLE_${cmake_package}")
      string(TOUPPER ${tmp} tmp)
      add_compile_definitions(${tmp})
      message(STATUS "[xTools-${module_name}]${module_name} imported successfully: ${lib_name}")
    else()
      x_remove_all_files_of_qt_optional(${module_name})
      message(WARNING "[xTools-${module_name}]${module_name} import failed.")
    endif()
  else()
    add_subdirectory(${X_3RD_DIR}/${module_name}-${QT_VERSION})
    add_custom_target(${module_name}_auto_install
      COMMAND ${CMAKE_COMMAND} --install . --prefix ${X_LIBS_DIR}/${module_name}-${QT_VERSION}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${module_name}-${QT_VERSION})
    set_property(TARGET ${module_name}_auto_install PROPERTY FOLDER "qt")
  endif()
  # cmake-format: on
endfunction()

# --------------------------------------------------------------------------------------------------
# Optional modules
if(QT_VERSION VERSION_LESS 6.5.0)
  return()
endif()

if(IOS OR ANDROID)
  message(STATUS "[xTools] Skipping optional Qt modules on mobile platform.")
  return()
endif()

if(APPLE)
  message(STATUS "[xTools] Skipping optional Qt modules on macOS platform.")
  return()
endif()

option(X_ENABLE_OPTIONAL_QT_MODULES "Enable optional Qt modules (Qt 6.5+)" OFF)
if(NOT X_ENABLE_OPTIONAL_QT_MODULES)
  message(STATUS "[xTools] Skipping optional Qt modules.")
  return()
endif()

# * x_import_qt_optional_module("qtmqtt" "Mqtt")
# * x_import_qt_optional_module("qtcoap" "Coap")
# * x_import_qt_optional_module("qtopcua" "OpcUa")
