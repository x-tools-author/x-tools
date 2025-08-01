file(GLOB_RECURSE X_COMMON_FILES "${CMAKE_SOURCE_DIR}/src/common/xtools.*")
file(GLOB_RECURSE X_SOURCES ${CMAKE_SOURCE_DIR}/xapps/x/*.*)
file(GLOB_RECURSE BAR_CODE_SOURCES ${CMAKE_SOURCE_DIR}/src/tools/barcode/*.*)
file(GLOB_RECURSE XCODE_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.*)

file(GLOB_RECURSE X_COMMON_FILES "${CMAKE_SOURCE_DIR}/src/common/xtools.*")
list(APPEND XCODE_SOURCES ${X_COMMON_FILES})
file(GLOB_RECURSE X_COMMON_FILES "${CMAKE_SOURCE_DIR}/src/common/escape.*")
list(APPEND XCODE_SOURCES ${X_COMMON_FILES})
file(GLOB_RECURSE X_COMMON_FILES "${CMAKE_SOURCE_DIR}/src/common/crc.*")
list(APPEND XCODE_SOURCES ${X_COMMON_FILES})

list(APPEND XCODE_SOURCES ${X_SOURCES})
list(APPEND XCODE_SOURCES ${BAR_CODE_SOURCES})
list(APPEND XCODE_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xCode.rc)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/xCode)

include_directories(${CMAKE_SOURCE_DIR}/xapps/x)
qt_add_executable(xCode MANUAL_FINALIZATION ${XCODE_SOURCES})
target_link_libraries(
  xCode
  PRIVATE Qt6::Core
          Qt6::Gui
          Qt6::Svg
          Qt6::Widgets
          Qt6::Network
          Qt6::SerialPort
          ${X_ICONV_LIBS})
target_link_libraries(xCode PRIVATE ${X_ZINT_LIBS})
x_deploy_qt(xCode)
if(WIN32 AND MSVC)
  target_link_libraries(xCode PRIVATE Dwmapi)
elseif(LINUX)
  target_link_libraries(xCode PRIVATE dl)
endif()
