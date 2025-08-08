# https://codeload.github.com/paceholder/nodeeditor/zip/refs/tags/3.0.12
set(packet_version 3.0.12)
set(packet_name nodeeditor-${packet_version})
set(packet_url "https://codeload.github.com/paceholder/nodeeditor/zip/refs/tags/${packet_version}")

macro(remove_all_files_of_flow)
  message(STATUS "[xFlow]Removing all flow files...")
  file(GLOB_RECURSE files "${CMAKE_SOURCE_DIR}/src/flow/*.*")
  foreach(file ${files})
    list(REMOVE_ITEM X_SOURCES file)
    message(STATUS "[xFlow]Removed file: ${file}")
  endforeach()
endmacro()

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

x_auto_import_package(${packet_name} QtNodes)
set(X_ENABLE_FLOW true)
add_compile_definitions(X_FLOW_ENABLED)
list(APPEND X_LIBS QtNodes::QtNodes)
