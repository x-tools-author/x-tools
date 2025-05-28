# cmake-format: off
# --------------------------------------------------------------------------------------------------
# Do something make CMake happy
if(NOT ${CMAKE_VERSION} VERSION_LESS "4.0.0")
  set(CMAKE_POLICY_VERSION_MINIMUM "3.22" CACHE STRING "CMake policy version" FORCE)
endif()

cmake_minimum_required(VERSION 3.22)
project(xPing VERSION 1.0 LANGUAGES C CXX)
# cmake-format: on

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Qml Quick QuickControls2)

file(GLOB X_PING_QML ${CMAKE_CURRENT_LIST_DIR}/qml/*.*)
file(GLOB X_PING_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.*)
list(APPEND X_PING_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xPing.rc)
list(APPEND X_PING_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xPing.qrc)
list(APPEND X_PING_SOURCES ${X_PING_SOURCES} ${X_PING_QML})

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
add_executable(xPing ${X_PING_SOURCES})
x_deploy_qt(xPing)
x_generate_translations(xPing)
x_output_env(xPing)
x_deploy_qt(xPing)

target_link_libraries(xPing PRIVATE ${X_TOOLS_LIBS} Qt6::Widgets Qt6::Qml Qt6::Quick
                                    Qt6::QuickControls2)

if(X_USING_VS_CODE)
  set_target_properties(xPing PROPERTIES MACOSX_BUNDLE TRUE)
else()
  set_target_properties(xPing PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
endif()

# --------------------------------------------------------------------------------------------------
# Make installer for Windows
if(WIN32 AND X_TOOLS_LATEST_GIT_TAG)
  set(icon ${CMAKE_CURRENT_LIST_DIR}/xPing.ico)
  x_tools_generate_zip(xPing ${X_TOOLS_LATEST_GIT_TAG})
  x_tools_generate_msix(xPing "xPing" "xPing" ${X_TOOLS_LATEST_GIT_TAG} FALSE)
  x_tools_generate_installer(xPing ${X_TOOLS_LATEST_GIT_TAG} ${icon})
endif()
