find_package(Qt6 REQUIRED COMPONENTS Qml Quick QuickControls2)
set(X_BASE_PATH ${CMAKE_SOURCE_DIR}/xapps/xping)
file(GLOB_RECURSE X_PING_SOURCES ${X_BASE_PATH}/src/*.*)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

include(cmake/x_qt_deploy.cmake)
include(cmake/x_qt_linguist.cmake)

set(bin ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/xPing)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin})
qt_add_executable(xPing ${X_PING_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/xPing.rc
                  ${CMAKE_CURRENT_LIST_DIR}/xPing.qrc)
x_output_env(xPing)
x_deploy_qt(xPing)

target_link_libraries(xPing PRIVATE Qt::Widgets Qt::Qml Qt::Quick Qt::QuickControls2)
if(X_USING_VS_CODE)
  set_target_properties(xPing PROPERTIES MACOSX_BUNDLE TRUE)
else()
  set_target_properties(xPing PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
endif()

# --------------------------------------------------------------------------------------------------
# Make installer for Windows
set(X_PING_VERSION "2.1.0")
add_compile_definitions(X_PING_VERSION="${X_PING_VERSION}")
if(WIN32)
  include(${CMAKE_SOURCE_DIR}/cmake/msix/msix.cmake)
  include(${CMAKE_SOURCE_DIR}/cmake/qifw/qifw.cmake)
  set(icon ${CMAKE_CURRENT_LIST_DIR}/xPing.ico)
  x_generate_zip(xPing ${X_PING_VERSION})
  x_generate_msix(xPing "xPing" "xPing" ${X_PING_VERSION} FALSE)
  x_generate_installer(xPing ${X_PING_VERSION} ${icon})
endif()
