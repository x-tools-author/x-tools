# https://codeload.github.com/paceholder/nodeeditor/zip/refs/tags/3.0.12
set(packet_version 3.0.12)
set(packet_name nodeeditor-${packet_version})
set(packet_url "https://codeload.github.com/paceholder/nodeeditor/zip/refs/tags/${packet_version}")

macro(remove_all_files_of_flow)
  message(STATUS "[xTools-nodeeditor] Removing all node editor files...")
  file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/nodeeditor/*.*")
  foreach(file ${files})
    list(REMOVE_ITEM X_SOURCES ${file})
    # message(STATUS "[xTools-nodeeditor] Removed file: ${file}")
  endforeach()
endmacro()

option(X_ENABLE_X_NODE_EDITOR "Enable xFlow (NodeEditor) support" OFF)
if(NOT X_ENABLE_X_NODE_EDITOR)
  remove_all_files_of_flow()
  return()
endif()

# Not supported on Android or iOS
if(ANDROID OR IOS)
  remove_all_files_of_flow()
  return()
endif()

# Not supported on Qt versions less than 6.8.0
if(QT_VERSION VERSION_LESS "6.8.0")
  remove_all_files_of_flow()
  return()
endif()

# Just for windows now
if(NOT WIN32)
  remove_all_files_of_flow()
  return()
endif()

# Download the packet if it doesn't exist
if(NOT EXISTS "${X_3RD_DIR}/${packet_name}.zip")
  file(
    DOWNLOAD ${packet_url} ${X_3RD_DIR}/${packet_name}.zip
    SHOW_PROGRESS
    STATUS download_status)
  if(download_status EQUAL 0)
    message(STATUS "Downloaded ${packet_name}.zip successfully.")
  else()
    message(WARNING "Failed to download ${packet_name}.zip.")
    remove_all_files_of_flow()
    return()
  endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS Qml)
x_auto_import_package(${packet_name} QtNodes)
set(X_ENABLE_X_NODE_EDITOR true)
add_compile_definitions(X_ENABLE_X_NODE_EDITOR)
list(APPEND X_LIBS QtNodes::QtNodes Qt6::Qml)
message(STATUS "[xTools-nodeeditor] Enabled xFlow support.")
include_directories(${CMAKE_SOURCE_DIR}/src/nodeeditor)

# Qt-AES
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_qtaes.cmake)
