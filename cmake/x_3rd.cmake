function(x_auto_import_package package_zip_name package_name)
  # Extract the package name from the path
  if(NOT EXISTS ${X_3RD_DIR}/${package_zip_name})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${package_zip_name}.zip
                    WORKING_DIRECTORY ${X_3RD_DIR})
  endif()

  x_auto_import_package_dir(${package_zip_name} ${package_name} ${ARGN})
endfunction()

function(x_auto_import_package_dir package_dir_name package_name)
  # Add module...
  set(package_dst_dir ${X_LIBS_DIR}/${package_dir_name})
  if(EXISTS ${package_dst_dir}/include)
    set(CMAKE_PREFIX_PATH ${package_dst_dir} ${CMAKE_PREFIX_PATH})
    find_package(${package_name} REQUIRED)
    message(STATUS "[PUMA]Found ${package_dir_name}: ${package_dst_dir}")
  else()
    add_subdirectory(${X_3RD_DIR}/${package_dir_name})

    if(NOT ANDROID AND NOT IOS)
      set(target_name "${package_dir_name}_auto_install")
      if(TARGET ${target_name})
        add_custom_target(
          ${target_name} ALL
          COMMAND ${CMAKE_COMMAND} --install . --prefix ${package_dst_dir}
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${package_dir_name}
          COMMENT "Installing ${package_dir_name} to ${package_dst_dir}")
      endif()
      if(TARGET ${target_name})
        add_dependencies(${target_name} ${package_name} ${ARGN})
      endif()
      set_property(TARGET ${package_dir_name}_auto_install PROPERTY FOLDER "3rd")
    endif()
  endif()
endfunction()

option(X_DOWNLOAD_FROM_GITEE "Download 3rd party libraries from Gitee" OFF)
include(FetchContent)

include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_lua.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_glog.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_coap.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_zint.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_qxlsx.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_opcua.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_hidapi.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_libiconv.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_mongoose.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_nodeeditor.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_qcustomplot.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_qmdnsengine.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_libqrencode.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_3rd_singleapplication.cmake)
