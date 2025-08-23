# https://github.com/fukuchi/libqrencode
set(qrencode_package_name "libqrencode-master")

set(X_QR_ENCODE OFF)
# --------------------------------------------------------------------------------------------------
# Extract libencode zip...
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${qrencode_package_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${qrencode_package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
# Add libencode module...
if(NOT DEFINED CMAKE_BUILD_TYPE)
  return()
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  set(X_QRENCODE_LIB "qrencoded")
else()
  set(X_QRENCODE_LIB "qrencode")
endif()

set(X_QR_ENCODE ON)
add_compile_definitions(X_QR_ENCODE)
message(STATUS "[qrencode] X_QRENCODE_LIB: ${X_QRENCODE_LIB}")
set(qrencode_dst_dir ${X_LIBS_DIR}/${qrencode_package_name})
if(EXISTS ${qrencode_dst_dir})
  link_directories(${qrencode_dst_dir}/lib)
  include_directories(${qrencode_dst_dir}/include)
  message(STATUS "[qrencode] Using existing qrencode in ${qrencode_dst_dir}")
else()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/${qrencode_package_name})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/${qrencode_package_name})
  link_directories(${CMAKE_BINARY_DIR}/3rd/${qrencode_package_name})
  set_property(TARGET qrencode PROPERTY FOLDER "3rd")

  add_custom_command(
    OUTPUT ${X_LIBS_DIR}/${qrencode_package_name}/install.stamp
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${X_LIBS_DIR}/${qrencode_package_name}
    COMMAND ${CMAKE_COMMAND} -E touch ${X_LIBS_DIR}/${qrencode_package_name}/install.stamp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${qrencode_package_name}
    COMMENT "Installing qrencode to ${X_LIBS_DIR}/${qrencode_package_name}")
  add_custom_target(qrencode_auto_install ALL
                    DEPENDS ${X_LIBS_DIR}/${qrencode_package_name}/install.stamp)
  add_dependencies(qrencode_auto_install qrencode)
  set_property(TARGET qrencode_auto_install PROPERTY FOLDER "3rd")
endif()

list(APPEND X_LIBS ${X_QRENCODE_LIB})
