# https://github.com/lua/lua
set(lua_version "5.4.8")
get_filename_component(COMPILER_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
message(STATUS "[lua] Compiler dir: ${COMPILER_DIR}")

# Download lua: https://codeload.github.com/lua/lua/zip/refs/tags/v5.4.8
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}.zip)
  message(STATUS "[lua] Downloading lua")
  set(lua_url "https://codeload.github.com/lua/lua/zip/refs/tags/v${lua_version}")
  file(
    DOWNLOAD ${lua_url} ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}.zip
    SHOW_PROGRESS
    STATUS lua_download_status)
  if(NOT lua_download_status EQUAL 0)
    message(WARNING "[lua] Failed to download lua, lua functionality will be disabled")
    return()
  endif()
endif()

# Extract lua
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version})
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}.zip
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}/src)
set(lua_lib_file ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/${CMAKE_C_COMPILER_VERSION}/lua.lib)
get_filename_component(lua_lib_dir ${lua_lib_file} DIRECTORY)

if(EXISTS ${lua_lib_file})
  link_directories(${lua_lib_dir})
else()
  file(GLOB LUA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}/src/*.*)
  list(REMOVE_ITEM LUA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua-${lua_version}/src/lua.c)
  add_library(lua STATIC ${LUA_SRC})
  set_target_properties(lua PROPERTIES FOLDER "3rd")
  set_target_properties(lua PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${lua_lib_dir})
endif()

# cmake-format: off
set(X_LUA ON CACHE BOOL "Use Lua" FORCE)
set(xLuaLib "lua" CACHE STRING "Lua library name" FORCE)
add_compile_definitions(X_LUA)
# cmake-format: on
