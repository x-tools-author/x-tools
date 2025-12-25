# --------------------------------------------------------------------------------------------------
# Find Qt5
find_package(QT NAMES Qt5 REQUIRED)
find_package(
  Qt5
  COMPONENTS SVG Widgets Network SerialPort WebSockets
  REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  add_compile_options(/execution-charset:utf-8)
endif()

# --------------------------------------------------------------------------------------------------
# Get all source files
file(GLOB_RECURSE X_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/legacy/src/*.*)
list(APPEND tmp_dirs "common" "device" utilities)
foreach(tmp_dir ${tmp_dirs})
  set(dir ${CMAKE_CURRENT_SOURCE_DIR}/src/${tmp_dir})
  file(GLOB_RECURSE src_files ${dir}/*.*)
  list(APPEND X_SOURCES ${src_files})
endforeach()

# --------------------------------------------------------------------------------------------------
# Remove useless files
list(FILTER X_SOURCES EXCLUDE REGEX ".*sctp.*")
list(FILTER X_SOURCES EXCLUDE REGEX "ble.*")
list(FILTER X_SOURCES EXCLUDE REGEX "charts.*")
list(FILTER X_SOURCES EXCLUDE REGEX "gps.*")
list(FILTER X_SOURCES EXCLUDE REGEX "hid.*")
list(FILTER X_SOURCES EXCLUDE REGEX "local.*")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(xTools ${X_SOURCES})
target_link_libraries(xTools PRIVATE Qt5::Svg Qt5::Widgets Qt5::Network Qt5::SerialPort
                                     Qt5::WebSockets)
add_compile_definitions(X_DISABLE_LOG)
add_compile_definitions(X_ENABLE_SERIALPORT=1)
