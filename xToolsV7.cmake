# --------------------------------------------------------------------------------------------------
# Get all source files of the project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE X_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.*")
list(APPEND X_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/xTools.qrc)

include(${CMAKE_SOURCE_DIR}/cmake/x_3rd.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt.cmake)

# --------------------------------------------------------------------------------------------------
# if X_LATEST_GIT_TAG contain "v", remove it...
if(NOT DEFINED X_LATEST_GIT_TAG)
  set(X_LATEST_GIT_TAG "9.9.9")
endif()

if(X_LATEST_GIT_TAG MATCHES "^v[0-9].*")
  string(SUBSTRING ${X_LATEST_GIT_TAG} 1 -1 X_LATEST_GIT_TAG)
endif()

# --------------------------------------------------------------------------------------------------
# xApplications selector(just for Qt6.8.0 or later)
include(${CMAKE_SOURCE_DIR}/xapps/xApp.cmake)
if(NOT ${X_APP} STREQUAL "xTools")
  return()
endif()

# --------------------------------------------------------------------------------------------------
# xTools application
set(bin ${X_BINS_DIR}/xTools)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin})

# --------------------------------------------------------------------------------------------------
# Apple platforms settings
if(APPLE)
  set(MACOSX_BUNDLE_ICON_FILE xTools.icns)
  set(xTools_ICON ${CMAKE_SOURCE_DIR}/xTools.icns)
  set_source_files_properties(${xTools_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  list(APPEND X_SOURCES ${xTools_ICON})
endif()

if(WIN32)
  list(APPEND X_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/xTools.rc)
endif()

# Inner xApps
include(${CMAKE_SOURCE_DIR}/cmake/x_apps.cmake)

# xTools
x_add_executable(xTools ${X_SOURCES})
target_link_libraries(xTools PRIVATE ${X_LIBS})
x_output_env(xTools)
x_deploy_qt(xTools)
x_deploy_resources(xTools)

# --------------------------------------------------------------------------------------------------
# xTools translations(i18n)
x_generate_module_translations(xtools ${CMAKE_CURRENT_SOURCE_DIR}/src OFF)

# Page modules
x_generate_module_translations(page ${CMAKE_CURRENT_SOURCE_DIR}/src/page OFF)
x_generate_module_translations(page-common ${CMAKE_CURRENT_SOURCE_DIR}/src/page/common ON)
x_generate_module_translations(page-emitter ${CMAKE_CURRENT_SOURCE_DIR}/src/page/emitter ON)
x_generate_module_translations(page-panels ${CMAKE_CURRENT_SOURCE_DIR}/src/page/panels ON)
x_generate_module_translations(page-preset ${CMAKE_CURRENT_SOURCE_DIR}/src/page/preset ON)
x_generate_module_translations(page-responder ${CMAKE_CURRENT_SOURCE_DIR}/src/page/responder ON)
x_generate_module_translations(page-transfer ${CMAKE_CURRENT_SOURCE_DIR}/src/page/transfer ON)
x_generate_module_translations(page-utilities ${CMAKE_CURRENT_SOURCE_DIR}/src/page/utilities ON)
x_generate_module_translations(page-scripts ${CMAKE_CURRENT_SOURCE_DIR}/src/page/scripts ON)

# Tools modules
x_generate_module_translations(tools ${CMAKE_CURRENT_SOURCE_DIR}/src/tools OFF)
x_generate_module_translations(tools-ascii ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/ascii ON)
x_generate_module_translations(tools-barcode ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/barcode ON)
x_generate_module_translations(tools-base64 ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/base64 ON)
x_generate_module_translations(tools-broadcast ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/broadcast ON)
x_generate_module_translations(tools-canbus ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/canbus ON)
x_generate_module_translations(tools-crc ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/crc ON)
x_generate_module_translations(tools-filecheck ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/filecheck ON)
x_generate_module_translations(tools-mdns ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/mdns ON)
x_generate_module_translations(tools-modbus ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/modbus ON)
x_generate_module_translations(tools-number ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/number ON)
x_generate_module_translations(tools-ping ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/ping ON)
x_generate_module_translations(tools-qrcode ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/qrcode ON)
x_generate_module_translations(tools-string ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/string ON)

# Other modules
x_generate_module_translations(common ${CMAKE_CURRENT_SOURCE_DIR}/src/common ON)
x_generate_module_translations(device ${CMAKE_CURRENT_SOURCE_DIR}/src/device ON)
x_generate_module_translations(nodeeditor ${CMAKE_CURRENT_SOURCE_DIR}/src/nodeeditor ON)
