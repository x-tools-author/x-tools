find_package(Qt6 REQUIRED COMPONENTS Qml Quick QuickControls2)

file(GLOB X_PING_QML ${CMAKE_CURRENT_LIST_DIR}/qml/*.*)
file(GLOB X_PING_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.*)
list(APPEND X_PING_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xPing.rc)
list(APPEND X_PING_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xPing.qrc)
list(APPEND X_PING_SOURCES ${X_PING_SOURCES} ${X_PING_QML})

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
add_executable(xPing ${X_PING_SOURCES})
x_deploy_qt(xPing)
x_generate_translations(xPing)
x_output_env(xPing)

target_link_libraries(xPing PRIVATE ${X_TOOLS_LIBS} Qt6::Widgets Qt6::Qml Qt6::Quick
                                    Qt6::QuickControls2)
if(X_USING_VS_CODE)
  set_target_properties(xPing PROPERTIES MACOSX_BUNDLE TRUE)
else()
  set_target_properties(xPing PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
endif()

# --------------------------------------------------------------------------------------------------
# Make installer for Windows
set(X_PING_VERSION "2.1.0")
if(WIN32)
  include(${CMAKE_SOURCE_DIR}/cmake/msix/msix.cmake)
  include(${CMAKE_SOURCE_DIR}/cmake/qifw/qifw.cmake)
  set(icon ${CMAKE_CURRENT_LIST_DIR}/xPing.ico)
  x_generate_zip(xPing ${X_PING_VERSION})
  x_generate_msix(xPing "xPing" "xPing" ${X_PING_VERSION} FALSE)
  x_generate_installer(xPing ${X_PING_VERSION} ${icon})
endif()
