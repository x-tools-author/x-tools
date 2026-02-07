# * https://github.com/yhirose/cpp-httplib
# * https://github.com/yhirose/cpp-httplib/releases/download/v1.4.14/https.h
# * https://github.com/yhirose/cpp-httplib/releases/download/v1.4.14/https.c
macro(x_remove_all_http_files)
  file(GLOB_RECURSE http_files ${CMAKE_SOURCE_DIR}/src/x/http/*.*)
  foreach(file_path ${http_files})
    message(STATUS "[xTools.http]Removing http file: ${file_path}")
    list(REMOVE_ITEM X_SOURCES ${file_path})
  endforeach()
  add_compile_definitions(X_ENABLE_X_HTTP=0)
  return()
endmacro()

if(ANDROID OR IOS)
  x_remove_all_http_files()
  return()
endif()

option(X_ENABLE_X_HTTP "Enable http support via Qt http module" OFF)
if(NOT X_ENABLE_X_HTTP)
  x_remove_all_http_files()
  return()
endif()

# --------------------------------------------------------------------------------------------------
set(package_version v0.30.2)
set(package_name https-${package_version})
set(base_url https://github.com/yhirose/cpp-httplib/releases/download/${package_version})
set(src_dir ${X_3RD_DIR}/${package_name})

if(NOT EXISTS ${src_dir}/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -E rm -rf ${package_name} || ${CMAKE_COMMAND} -E true
                  WORKING_DIRECTORY ${X_3RD_DIR})
  execute_process(COMMAND git clone https://github.com/yhirose/cpp-httplib.git ${package_name}
                  WORKING_DIRECTORY ${X_3RD_DIR})
  execute_process(COMMAND git switch -c ${package_version} WORKING_DIRECTORY ${src_dir})
endif()

if(NOT EXISTS ${src_dir}/CMakeLists.txt)
  x_remove_all_http_files()
  return()
endif()

add_compile_definitions(X_ENABLE_X_HTTP=1)
# Using pre-builded library
set(out_dir "${X_LIBS_DIR}/${package_name}")
if(EXISTS "${out_dir}/lib/libhttp.a" OR EXISTS "${out_dir}/lib/libhttp.lib")
  # * find_package(http REQUIRED PATHS "${out_dir}/lib/cmake/http" NO_DEFAULT_PATH)
  set(CMAKE_PREFIX_PATH ${out_dir} ${CMAKE_PREFIX_PATH})
  find_package(http REQUIRED)
  list(APPEND X_LIBS http::http)
  return()
else()
  add_subdirectory(${X_3RD_DIR}/${package_name})
  add_custom_target(
    libhttp-deploy
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${out_dir}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${package_name})
  set_target_properties(libhttp-deploy PROPERTIES FOLDER "3rd")
  list(APPEND X_LIBS http::http)
endif()
