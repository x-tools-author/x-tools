# https://github.com/lua/lua
set(lua_version "5.4.8")
get_filename_component(COMPILER_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
message(STATUS "[lua] Compiler dir: ${COMPILER_DIR}")

# Download lua: https://github.com/lua/lua/archive/refs/tags/v5.4.8.zip
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}.zip)
  message(STATUS "[lua] Downloading lua")
  set(lua_url "https://github.com/lua/lua/archive/refs/tags/v${lua_version}.zip")
  file(
    DOWNLOAD ${lua_url} ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}.zip
    SHOW_PROGRESS
    STATUS lua_download_status)
  if(NOT lua_download_status EQUAL 0)
    message(WARNING "[lua] Failed to download lua, lua functionality will be disabled")
    file(GLOB LUA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/common/luarunner*")
    foreach(file ${LUA_FILES})
      list(REMOVE_ITEM X_SOURCES ${file})
      message(STATUS "[Lua]Remove file: ${file}")
    endforeach()
    return()
  endif()
endif()

# Extract lua
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version})
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}.zip
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

set(lua_root ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version})
include_directories(${lua_root})
message(STATUS "[lua] Include dir: ${lua_root}")

if(MSVC)
  set(lua_lib_file ${X_LIBS_DIR}/lua-${lua_version}/liblua.lib)
elseif(APPLE)
  set(lua_lib_file ${X_LIBS_DIR}/lua-${lua_version}/liblua.a)
else()
  set(lua_lib_file ${X_LIBS_DIR}/lua-${lua_version}/lua.a)
endif()

get_filename_component(lua_lib_dir ${lua_lib_file} DIRECTORY)

file(GLOB LUA_H ${lua_root}/*.h)
file(GLOB LUA_C ${lua_root}/*.c)
list(REMOVE_ITEM LUA_C ${lua_root}/lua.c)
list(REMOVE_ITEM LUA_C ${lua_root}/onelua.c)
set(LUA_FILES ${LUA_H} ${LUA_C})

if(WIN32)
  set(lua_target liblua)
else()
  set(lua_target lua)
endif()

if(EXISTS ${lua_lib_file})
  link_directories(${lua_lib_dir})
else()
  add_library(${lua_target} STATIC ${LUA_FILES})
  set_target_properties(${lua_target} PROPERTIES FOLDER "3rd")
  set_target_properties(${lua_target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lua_lib_dir})
endif()

add_compile_definitions(X_ENABLE_LUA)
list(APPEND X_LIBS ${lua_target})

option(X_ENABLE_LUA_APP "Enable Lua support" OFF)
if(MSVC AND X_ENABLE_LUA_APP)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${X_BINS_DIR}/lua)
  add_executable(lua ${lua_root}/onelua.c)
  # cmake-format: off
  set_target_properties(lua PROPERTIES CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  # cmake-format: on
  if(EXISTS ${lua_root}/luac.c) # 这个文件从官网下载的源码包才有，github 上的源码包没有
    add_executable(luac ${lua_root}/onelua.c)
    target_compile_definitions(luac PRIVATE MAKE_LUAC)
    # cmake-format: off
    set_target_properties(luac PROPERTIES CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # cmake-format: on
  endif()
endif()

function(x_deploy_lua TARGET)
  set(dst_dir $<TARGET_FILE_DIR:${TARGET}>/scripts)
  if(APPLE)
    set(dst_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../Resources/scripts)
  endif()
  make_directory(${dst_dir})
  add_custom_command(
    TARGET ${TARGET}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res/scripts ${dst_dir}
    COMMENT "Copy lua scripts to output dir")
endfunction()
