# * https://github.com/sctplab/usrsctp.git
set(package_version 0.9.5.0)

macro(x_remove_all_sctp_files)
  file(GLOB_RECURSE BLE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/sctp*")
  foreach(file ${BLE_FILES})
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[xTools.SCTP] Remove SCTP file: ${file}")
  endforeach()
  add_compile_definitions(X_ENABLE_SCTP=0)
endmacro()

if(ANDROID OR IOS)
  x_remove_all_sctp_files()
  return()
endif()

option(X_ENABLE_SCTP "Enable SCTP support via usrsctp library" OFF)
if(NOT X_ENABLE_SCTP)
  x_remove_all_sctp_files()
  return()
endif()

add_compile_definitions(X_ENABLE_SCTP=1)
if(WIN32)
  file(GLOB_RECURSE SCTP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/device/platforms/windows/sctp*")
  foreach(file ${SCTP_FILES})
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[xTools.SCTP] Remove SCTP file: ${file}")
  endforeach()
endif()

# Using pre-build usrsctp libs
set(dst_dir ${X_LIBS_DIR}/usrsctp-${package_version})
if(EXISTS ${dst_dir}/include/usrsctp.h)
  include_directories(${dst_dir}/include)
  link_directories(${dst_dir}/lib)
  list(APPEND X_LIBS usrsctp)
  return()
endif()

# Build usrsctp from source
set(sctp_werror
    OFF
    CACHE BOOL "Disable Werror in usrsctp" FORCE)
FetchContent_Declare(
  usrsctp
  GIT_REPOSITORY https://github.com/sctplab/usrsctp.git
  GIT_TAG ${package_version})
FetchContent_MakeAvailable(usrsctp)
list(APPEND X_LIBS usrsctp)

# Install usrsctp
add_custom_target(
  usrsctp_post ALL
  DEPENDS usrsctp
  COMMAND ${CMAKE_COMMAND} --install . --prefix ${dst_dir}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/usrsctp-build
  COMMENT "Installing usrsctp to ${dst_dir}")
