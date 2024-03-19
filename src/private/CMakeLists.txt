include(${CMAKE_SOURCE_DIR}/.cmake/sak_common.cmake)

file(
  GLOB dirs
  LIST_DIRECTORIES true
  "${CMAKE_CURRENT_SOURCE_DIR}/*")
list(REMOVE_ITEM dirs "common")
set(SAK_HAS_PRIVATE_MODULE false)

foreach(dir ${dirs})
  if(IS_DIRECTORY ${dir})
    if(NOT ${SAK_HAS_PRIVATE_MODULE})
      set(SAK_HAS_PRIVATE_MODULE true)
    endif()
  endif()
endforeach()

foreach(dir ${dirs})
  if(IS_DIRECTORY ${dir})
    file(RELATIVE_PATH cooked_dir ${CMAKE_SOURCE_DIR} ${dir})
    add_subdirectory(${CMAKE_SOURCE_DIR}/${cooked_dir})
  endif()
endforeach()

set(GITEE_URL "git clone https://gitee.com/qsaker-qsak")
add_custom_target(
  ClonePrivate
  COMMAND git clone ${GITEE_URL}/keygen.git || echo "clone keygen"
  COMMAND git clone ${GITEE_URL}/fluent2.git || echo "clone fluent2"
  COMMAND git clone ${GITEE_URL}/easydebug.git || echo "clone easydebug"
  COMMAND git clone ${GITEE_URL}/dataflowstudio.git || echo "clone dataflowstudio"
  COMMAND git clone ${GITEE_URL}/passwordassassin.git || echo "clone passwordassassin"
  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/readme.md
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/private)
