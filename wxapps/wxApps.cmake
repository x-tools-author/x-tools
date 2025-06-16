# * https://github.com/wxWidgets/wxWidgets/releases/download/v3.3.0/wxWidgets-3.3.0.zip
# * set(package_version "3.2.8.1")
set(package_version "3.3.0")
set(package_base_url "https://github.com/wxWidgets/wxWidgets/releases/download")
set(package_url "${package_base_url}/v${package_version}/wxWidgets-${package_version}.zip")
set(package_name "wxWidgets-${package_version}")

# --------------------------------------------------------------------------------------------------
# cmake-format: off
if(WIN32 AND MSVC)
  set(wxBUILD_SHARED OFF CACHE BOOL "Build wxWidgets as shared libraries" FORCE)
  set(wxBUILD_USE_STATIC_RUNTIME ON CACHE BOOL "Build static runtime" FORCE)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif(LINUX)
  set(wxBUILD_SHARED OFF CACHE BOOL "Build wxWidgets as shared libraries" FORCE)
  set(wxBUILD_USE_STATIC_RUNTIME ON CACHE BOOL "Build static runtime" FORCE)
endif()
# cmake-format: on

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  add_compile_definitions(X_DEBUG)
else()
  add_compile_definitions(X_RELEASE)
endif()

# --------------------------------------------------------------------------------------------------
# 3rd modules
include(${CMAKE_SOURCE_DIR}/cmake/glog.cmake)

# --------------------------------------------------------------------------------------------------
# Download the package if it does not exist
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${package_name}.zip)
  message(STATUS "Downloading ${package_name} from ${package_url}")
  file(
    DOWNLOAD ${package_url} ${CMAKE_SOURCE_DIR}/3rd/${package_name}.zip
    SHOW_PROGRESS
    STATUS status)
  if(NOT status EQUAL 0)
    message(FATAL_ERROR "Failed to download ${package_name} from ${package_url}")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Extract the package
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${package_name})
  message(STATUS "Extracting ${package_name}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${package_name}
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar "xzf" ${CMAKE_SOURCE_DIR}/3rd/${package_name}.zip
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd/${package_name}
    RESULT_VARIABLE result)
  if(result)
    message(FATAL_ERROR "Failed to extract ${package_name}")
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Import wxWidgets
set(wxWidget_LIBS ${X_DEPLOY_LIBS_DIR}/${package_name})
message(STATUS "[wxWidgets] wxWidget_LIBS ${wxWidget_LIBS}")
if(EXISTS ${wxWidget_LIBS})
  message(STATUS "Using pre-deployed wxWidgets from ${wxWidget_LIBS}")
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${wxWidget_LIBS})
  find_package(wxWidgets REQUIRED COMPONENTS core base aui)
  include(${wxWidgets_USE_FILE})
else()
  add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${package_name})
  x_install_3rd_library(wxWidgets ${package_name})
  set(wxWidgets_LIBRARIES wx::core wx::aui)
endif()

# --------------------------------------------------------------------------------------------------
# wxApps
file(GLOB_RECURSE wx_files "${CMAKE_CURRENT_LIST_DIR}/wx/*.*")
set(X_APPS "xCrc")
file(GLOB PRIVATE_APPS "${CMAKE_SOURCE_DIR}/wxapps/*")
foreach(private_app ${PRIVATE_APPS})
  if(NOT IS_DIRECTORY ${private_app})
    continue()
  endif()

  get_filename_component(app_name ${private_app} NAME)
  if(app_name STREQUAL "wx")
    continue()
  endif()

  # * The second char of the app name should be uppercase
  # * if app_name == xcrc, the char2 is 'c', it will be converted to 'C'
  # * if app_name == xcrc, the rest_of_name is 'rc'
  # * if app_name == xcrc, the app_name will be 'xCrc'(x + C + rc)
  string(SUBSTRING ${app_name} 2 1 char3)
  string(TOUPPER ${char3} char3)
  string(SUBSTRING "${app_name}" 3 -1 rest_of_name)
  string(CONCAT app_name "wx${char3}${rest_of_name}")

  # If X_APPS already contains the app_name, skip it
  if(NOT app_name IN_LIST X_APPS)
    list(APPEND X_APPS "${app_name}")
  endif()
endforeach()

# cmake-format: off
set(X_APP "xTools" CACHE STRING "Select a x-app to build")
set_property(CACHE X_APP PROPERTY STRINGS ${X_APPS})
string(TOLOWER ${X_APP} LOWER_X_APP)
include_directories(${CMAKE_SOURCE_DIR}/wxapps)
include(${CMAKE_SOURCE_DIR}/wxapps/${LOWER_X_APP}/${X_APP}.cmake)
# cmake-format: on
