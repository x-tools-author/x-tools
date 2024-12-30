function(x_tools_add_xlsx)
  set(xlsx "QXlsx-1.4.7")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${xlsx}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${xlsx}/QXlsx/header)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/${xlsx}/QXlsx)
endfunction()
