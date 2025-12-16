# https://github.com/lua/lua
set(lua_version "5.4.8")
get_filename_component(COMPILER_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
message(STATUS "[xTools.Lua] Compiler dir: ${COMPILER_DIR}")

# Download lua: https://github.com/lua/lua/archive/refs/tags/v5.4.8.zip
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}.zip)
  message(STATUS "[xTools.Lua] Downloading lua")
  set(lua_url "https://github.com/lua/lua/archive/refs/tags/v${lua_version}.zip")
  set(file_path ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}.zip)
  file(
    DOWNLOAD ${lua_url} ${file_path}
    SHOW_PROGRESS
    STATUS lua_download_status)
  if(NOT lua_download_status EQUAL 0)
    # Remove lua zip file
    file(REMOVE ${file_path})
    message(FATAL_ERROR "[xTools.Lua] Failed to download lua")
  endif()
endif()

# Extract lua
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version})
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}.zip
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
set(lua_root ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version})
include_directories(${lua_root})
message(STATUS "[xTools.Lua] Include dir: ${lua_root}")
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

# --------------------------------------------------------------------------------------------------
add_compile_definitions(X_ENABLE_LUA)
list(APPEND X_LIBS ${lua_target})
if(MSVC)
  set(lua_lib_file ${X_LIBS_DIR}/lua-${lua_version}/liblua.lib)
elseif(APPLE)
  set(lua_lib_file ${X_LIBS_DIR}/lua-${lua_version}/liblua.a)
else()
  set(lua_lib_file ${X_LIBS_DIR}/lua-${lua_version}/liblua.a)
endif()
get_filename_component(lua_lib_dir ${lua_lib_file} DIRECTORY)
if(EXISTS ${lua_lib_file})
  link_directories(${lua_lib_dir})
else()
  add_library(${lua_target} STATIC ${LUA_FILES})
  set_target_properties(${lua_target} PROPERTIES FOLDER "3rd")
  set_target_properties(${lua_target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lua_lib_dir})
endif()

# --------------------------------------------------------------------------------------------------
# Optional
option(X_ENABLE_LUA_APP "Enable Lua support" OFF)
if(MSVC AND X_ENABLE_LUA_APP)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${X_BINS_DIR}/lua)
  add_executable(lua ${lua_root}/onelua.c)
  set_target_properties(lua PROPERTIES CMAKE_MSVC_RUNTIME_LIBRARY
                                       "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  if(EXISTS ${lua_root}/luac.c) # 这个文件从官网下载的源码包才有，github 上的源码包没有
    add_executable(luac ${lua_root}/onelua.c)
    target_compile_definitions(luac PRIVATE MAKE_LUAC)
    set_target_properties(luac PROPERTIES CMAKE_MSVC_RUNTIME_LIBRARY
                                          "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()
