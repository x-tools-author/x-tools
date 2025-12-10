# * https://github.com/open62541/open62541
# * https://github.com/open62541/open62541/releases/download/v1.4.14/open62541.h
# * https://github.com/open62541/open62541/releases/download/v1.4.14/open62541.c
macro(x_remove_all_opcua_files)
  file(GLOB_RECURSE opcua_files ${CMAKE_SOURCE_DIR}/src/x/opcua/*.*)
  foreach(file_path ${opcua_files})
    message(STATUS "[xTools.OpcUa]Removing OPC UA file: ${file_path}")
    list(REMOVE_ITEM X_SOURCES ${file_path})
  endforeach()
  add_compile_definitions(X_ENABLE_OPC_UA=0)
  return()
endmacro()

if(ANDROID OR IOS)
  x_remove_all_opcua_files()
  return()
endif()

option(X_ENABLE_OPC_UA "Enable OPC UA support via Qt OPC UA module" OFF)
if(NOT X_ENABLE_OPC_UA)
  x_remove_all_opcua_files()
  return()
endif()

# --------------------------------------------------------------------------------------------------
set(package_version v1.4.14)
set(package_name open62541-${package_version})
set(base_url https://github.com/open62541/open62541/releases/download/${package_version})
set(src_dir ${X_3RD_DIR}/${package_name})

if(NOT EXISTS ${src_dir}/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -E rm -rf ${package_name} || ${CMAKE_COMMAND} -E true
                  WORKING_DIRECTORY ${X_3RD_DIR})
  execute_process(COMMAND git clone https://github.com/open62541/open62541.git ${package_name}
                  WORKING_DIRECTORY ${X_3RD_DIR})
  execute_process(COMMAND git switch -c ${package_version} WORKING_DIRECTORY ${src_dir})
endif()

if(NOT EXISTS ${src_dir}/CMakeLists.txt)
  x_remove_all_opcua_files()
  return()
endif()

add_compile_definitions(X_ENABLE_OPC_UA=1)
# Using pre-builded library
set(out_dir "${X_LIBS_DIR}/${package_name}")
if(EXISTS "${out_dir}/lib/libopen62541.a" OR EXISTS "${out_dir}/lib/open62541.lib")
  # * find_package(open62541 REQUIRED PATHS "${out_dir}/lib/cmake/open62541" NO_DEFAULT_PATH)
  set(CMAKE_PREFIX_PATH ${out_dir} ${CMAKE_PREFIX_PATH})
  find_package(open62541 REQUIRED)
  list(APPEND X_LIBS open62541::open62541)
  return()
else()
  add_subdirectory(${X_3RD_DIR}/${package_name})
  add_custom_target(
    libopen62541-deploy
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${out_dir}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/3rd/${package_name})
  set_target_properties(libopen62541-deploy PROPERTIES FOLDER "3rd")
  list(APPEND X_LIBS open62541::open62541)
endif()
