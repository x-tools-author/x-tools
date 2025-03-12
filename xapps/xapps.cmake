option(X_ENABLE_APPS "Enable apps" ON)
if(NOT X_ENABLE_APPS)
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

include(${CMAKE_CURRENT_LIST_DIR}/xassistant/xassistant.cmake)
