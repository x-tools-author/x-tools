# https://github.com/simpleble/simpleble.git
set(package_version v0.10.3)

macro(x_remove_all_simpleble_files)
  file(GLOB_RECURSE BLE_PERIPHERAL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/bleperipheral*")
  foreach(file ${BLE_PERIPHERAL_FILES})
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[xTools.simpleble] Remove simpleble file: ${file}")
  endforeach()
  add_compile_definitions(X_ENABLE_BLE_PERIPHERAL=0)
endmacro()

if(ANDROID OR IOS)
  x_remove_all_simpleble_files()
  return()
endif()

option(X_ENABLE_BLE_PERIPHERAL "Enable SimpleBLE support via simpleble library" OFF)
if(NOT X_ENABLE_BLE_PERIPHERAL)
  x_remove_all_simpleble_files()
  return()
endif()

add_compile_definitions(X_ENABLE_BLE_PERIPHERAL=1)

# Using pre-build simpleble libs
set(dst_dir ${X_LIBS_DIR}/simpleble-${package_version})
if(EXISTS ${dst_dir}/include/simpleble/SimpleBLE.h)
  list(APPEND CMAKE_PREFIX_PATH ${dst_dir})
  find_package(simpleble REQUIRED)
  message(STATUS "[xTools] Found simpleble: ${dst_dir}")
  list(APPEND X_LIBS simpleble::simpleble simpleble::simpleble-c)
  return()
endif()

# Build simpleble from source
FetchContent_Declare(
  simpleble
  GIT_REPOSITORY https://github.com/simpleble/simpleble.git
  GIT_TAG ${package_version}
  SOURCE_SUBDIR simpleble)
FetchContent_MakeAvailable(simpleble)
list(APPEND X_LIBS simpleble::simpleble simpleble::simpleble-c)

# Install simpleble
add_custom_target(
  simpleble_post ALL
  DEPENDS simpleble simpleble-c
  COMMAND ${CMAKE_COMMAND} --install . --prefix ${dst_dir}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/simpleble-build
  COMMENT "Installing simpleble to ${dst_dir}")
