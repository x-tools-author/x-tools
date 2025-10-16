# Do something for CMake 4.0.0 or later
if(NOT ${CMAKE_VERSION} VERSION_LESS "4.0.0")
  set(CMAKE_POLICY_VERSION_MINIMUM
      "3.22"
      CACHE STRING "CMake policy version" FORCE)
endif()

# Using vcpkg, you must set the environment variable VCPKG_ROOT first
option(X_USING_VCPKG "Using vcpkg" OFF)
if(X_USING_VCPKG)
  set(VCPKG_TARGET_TRIPLET
      "x64-windows-static"
      CACHE STRING "vcpkg target triplet")
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Please specify your own vcpkg file")
  if(${VCPKG_TARGET_TRIPLET} STREQUAL "x64-windows-static")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()

cmake_minimum_required(VERSION 3.22)
project(
  xTools
  VERSION 8.0
  LANGUAGES C CXX
  DESCRIPTION "xTools - All in one toolbox for developers and engineers")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# cmake-format: off
set(x_tools_framework_verson "Widgets")
list(APPEND x_tools_framework_verson "Qml")
set(X_TOOLS_FRAMEWORK "Widgets" CACHE STRING "The version of xTools framework to use")
set_property(CACHE X_TOOLS_FRAMEWORK PROPERTY STRINGS ${x_tools_framework_verson})
# cmake-format: on

if(${X_TOOLS_FRAMEWORK} STREQUAL "Widgets")
  include(xToolsV7.cmake)
else()
  include(xToolsV8.cmake)
endif()
