find_package(Qt6 REQUIRED COMPONENTS Qml Quick QuickControls2)

file(GLOB X_APP_SOURCES "${CMAKE_SOURCE_DIR}/src/application.*")
file(GLOB_RECURSE X_COMMON_SOURCES "${CMAKE_SOURCE_DIR}/src/common/*.*")
file(GLOB_RECURSE X_DEVICE_SOURCES "${CMAKE_SOURCE_DIR}/src/device/*.*")
foreach(file ${X_DEVICE_SOURCES})
  # Remove all *ui.* files
  if(${file} MATCHES ".*ui.*")
    list(REMOVE_ITEM X_DEVICE_SOURCES ${file})
  endif()
endforeach()

file(GLOB_RECURSE X_DEBUG_QML "${CMAKE_CURRENT_LIST_DIR}/qml/*.*")
file(GLOB_RECURSE X_DEBUG_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.*")
list(APPEND X_DEBUG_SOURCES ${X_APP_SOURCES} ${X_COMMON_SOURCES} ${X_DEVICE_SOURCES} ${X_DEBUG_QML})
list(APPEND X_DEBUG_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xDebug.qrc)
list(APPEND X_DEBUG_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xDebug.rc)

set(bin ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/xDebug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin})
qt_add_executable(xDebug MANUAL_FINALIZATION ${X_DEBUG_SOURCES})
target_link_libraries(xDebug PRIVATE ${X_LIBS} Qt6::Qml Qt6::Quick Qt6::QuickControls2)
