include(.cmake/sak_common.cmake)

file(
  GLOB dirs
  LIST_DIRECTORIES true
  "${CMAKE_SOURCE_DIR}/src/private/*")
list(REMOVE_ITEM dirs "common")
set(SAK_HAS_PRIVATE_MODULE false)

foreach(dir ${dirs})
  if(IS_DIRECTORY ${dir})
    if(NOT ${SAK_HAS_PRIVATE_MODULE})
      set(SAK_HAS_PRIVATE_MODULE true)
    endif()
  endif()
endforeach()

if(${SAK_HAS_PRIVATE_MODULE})
  sak_add_subdirectory("nodeeditor-3.0.10")
  add_compile_definitions(SAK_IMPORT_MODULE_PRIVATE)
endif()

foreach(dir ${dirs})
  if(IS_DIRECTORY ${dir})
    file(RELATIVE_PATH cooked_dir ${CMAKE_SOURCE_DIR} ${dir})
    if(NOT ${cooked_dir} STREQUAL "src/private/common")
      add_subdirectory(${cooked_dir})
    endif()
  endif()
endforeach()

add_custom_target(
  PullPrivateModules
  COMMAND git clone https://gitee.com/qsak/fluent2.git ./fluent2
  COMMAND git clone https://gitee.com/qsak/easydebug.git ./easydebug
  COMMAND git clone https://gitee.com/qsak/dataflowstudio.git ./dataflowstudio
  SOURCES ${CMAKE_SOURCE_DIR}/src/private/private.cmake
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/private)
