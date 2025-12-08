file(GLOB_RECURSE X_FLOW_FILES "${CMAKE_SOURCE_DIR}/src/x/flow/*.*")
file(GLOB_RECURSE X_DEVICE_FILES "${CMAKE_SOURCE_DIR}/src/device/*.*")
file(GLOB_RECURSE COMMON_FILES "${CMAKE_SOURCE_DIR}/src/common/*.*")
file(GLOB_RECURSE X_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.*")

list(APPEND X_SOURCES ${X_COMMON_FILES})
list(APPEND X_SOURCES ${X_FLOW_FILES})
list(APPEND X_SOURCES ${COMMON_FILES})
list(APPEND X_SOURCES ${X_DEVICE_FILES})
list(APPEND X_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xFlow.rc)
list(APPEND X_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xFlow.qrc)
list(APPEND X_SOURCES ${CMAKE_SOURCE_DIR}/xTools.qrc)

# Remove all pyplugin files from the build
foreach(file IN LISTS PRO_FILES)
  if(file MATCHES "pyplugin\\.h$" OR file MATCHES "pyplugin\\.cpp$")
    list(REMOVE_ITEM PRO_FILES ${file})
    message(STATUS "[xFlow] Removed file: ${file}")
  endif()
endforeach()

include(cmake/x_qt.cmake)
include(cmake/x_3rd.cmake)

# --------------------------------------------------------------------------------------------------
include_directories(${CMAKE_SOURCE_DIR}/xapps)
include_directories(${CMAKE_SOURCE_DIR}/xapps/xFlow/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${X_BINS_DIR}/xFlow)
qt_add_executable(xFlow ${X_SOURCES})
x_deploy_qt(xFlow)
x_using_x_tools_translations(xFlow)
target_link_libraries(xFlow PRIVATE Qt6::CorePrivate Qt6::Svg Qt6::Widgets Qt6::Qml)
target_link_libraries(xFlow PRIVATE ${X_LIBS})
if(X_USING_VS_CODE)
  set_target_properties(xFlow PROPERTIES MACOSX_BUNDLE TRUE)
else()
  set_target_properties(xFlow PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
endif()

# --------------------------------------------------------------------------------------------------
if(WIN32)
  target_link_libraries(xFlow PRIVATE Dwmapi)
elseif(LINUX)
  target_link_libraries(xFlow PRIVATE dl)
endif()
