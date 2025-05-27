option(X_ENABLE_X_APPS "Enable apps" OFF)
if(NOT X_ENABLE_X_APPS)
  return()
endif()

# Remove all main.cpp xTools.rc  files from X_TOOLS_SOURCES
set(X_APPS_SOURCES ${X_TOOLS_SOURCES})
foreach(source ${X_APPS_SOURCES})
  if(${source} MATCHES "main.cpp")
    list(REMOVE_ITEM X_APPS_SOURCES ${source})
  endif()
  if(${source} MATCHES "xTools.rc")
    list(REMOVE_ITEM X_APPS_SOURCES ${source})
  endif()
endforeach()

# --------------------------------------------------------------------------------------------------
# xAssistant
include(${CMAKE_CURRENT_LIST_DIR}/xassistant/xAssistant.cmake)

# --------------------------------------------------------------------------------------------------
# xDebug
if(NOT QT_VERSION VERSION_LESS "6.8.0")
  include(${CMAKE_CURRENT_LIST_DIR}/xdebug/xDebug.cmake)
endif()

# --------------------------------------------------------------------------------------------------
# xPing
if(NOT QT_VERSION VERSION_LESS "6.8.0")
  include(${CMAKE_CURRENT_LIST_DIR}/xping/xPing.cmake)
endif()
