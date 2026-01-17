macro(remove_all_qrcode_files)
  file(GLOB_RECURSE QRENCODE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/qrcode/*.*")
  foreach(file ${QRENCODE_SOURCE})
    list(REMOVE_ITEM X_SOURCES ${file})
    message(STATUS "[xTools.qrencode]Remove file: ${file}")
  endforeach(file ${QRENCODE_SOURCE})
endmacro()

if(QT_VERSION VERSION_LESS "5.9.0")
  remove_all_qrcode_files()
  add_compile_definitions(X_DISABLE_QR_CODE)
  return()
endif()

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
  if(EXISTS ${CMAKE_BINARY_DIR}/3rd/${qrencode_package_name}/qrencoded.lib AND WIN32)
    set(X_QRENCODE_LIB "qrencoded")
  else()
    if(LINUX AND NOT APPLE)
      set(X_QRENCODE_LIB "qrencode")
    else()
      set(X_QRENCODE_LIB "qrencode")
    endif()
  endif()
else()
  set(X_QRENCODE_LIB "qrencode")
endif()

set(X_QR_ENCODE ON)
add_compile_definitions(X_QR_ENCODE)
message(STATUS "[qrencode] X_QRENCODE_LIB: ${X_QRENCODE_LIB}")
set(qrencode_dst_dir ${X_LIBS_DIR}/${qrencode_package_name})
if(EXISTS ${qrencode_dst_dir})
  link_directories(${qrencode_dst_dir}/lib)
  link_directories(${qrencode_dst_dir}/lib64)
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
