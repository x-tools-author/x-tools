if(NOT Qt6_VERSION VERSION_LESS "6.5.0")
  if(EXISTS ${CMAKE_SOURCE_DIR}/Source/Private/Private)
    add_subdirectory(${CMAKE_SOURCE_DIR}/Source/Private/Private)
  endif()

  set(GITEE_URL "https://gitee.com/x-tools-author/x-tools-private.git")
  add_custom_target(
    ClonePrivateModules
    COMMAND git clone ${GITEE_URL} ./Private || echo "clone private modules"
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Source/Private)
  add_custom_target(
    PullPrivateModules
    COMMAND git pull ${GITEE_URL}
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Source/Private/Private)
endif()
