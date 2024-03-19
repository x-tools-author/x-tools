if(NOT Qt6_VERSION VERSION_LESS "6.5.0")
  if(EXISTS ${CMAKE_SOURCE_DIR}/src/private/private)
    add_subdirectory(${CMAKE_SOURCE_DIR}/src/private/private)

    add_custom_target(
      PullPrivateModules
      COMMAND git pull ${GITEE_URL}
      SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/readme.md
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/private/private)
  endif()

  set(GITEE_URL "https://gitee.com/qsaker/QtSwissArmyKnifePrivate.git")
  add_custom_target(
    ClonePrivateModules
    COMMAND git clone ${GITEE_URL} ./private || echo "clone private modules"
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/readme.md
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/private)
endif()
