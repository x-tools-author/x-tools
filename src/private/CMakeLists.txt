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

add_custom_target(
  PullPrivateModules
  COMMAND git clone https://gitee.com/qsaker-qsak/fluent2.git ./fluent2
  COMMAND git clone https://gitee.com/qsaker-qsak/easydebug.git ./easydebug
  COMMAND git clone https://gitee.com/qsaker-qsak/dataflowstudio.git ./dataflowstudio
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/private)
