find_package(Qt6 REQUIRED COMPONENTS Qml Quick QuickControls2)

file(GLOB X_APP_SOURCES "${CMAKE_SOURCE_DIR}/src/application.*")
file(GLOB_RECURSE X_COMMON_SOURCES "${CMAKE_SOURCE_DIR}/src/common/*.*")
file(GLOB X_PING_QML ${CMAKE_CURRENT_LIST_DIR}/qml/*.*)
file(GLOB X_PING_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.*)
list(APPEND X_PING_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xPing.rc)
list(APPEND X_PING_SOURCES ${CMAKE_CURRENT_LIST_DIR}/xPing.qrc)
list(APPEND X_PING_SOURCES ${X_APP_SOURCES} ${X_COMMON_SOURCES} ${X_PING_QML})

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
add_executable(xPing ${X_PING_SOURCES})
x_tools_deploy_qt(xPing)
x_tools_generate_translations(xPing)
x_tools_output_env(xPing)
x_tools_deploy_qt(xPing)

target_link_libraries(xPing PRIVATE ${X_TOOLS_LIBS} Qt6::Qml Qt6::Quick Qt6::QuickControls2)

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  set_target_properties(xPing PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
else()
  option(X_ASSISTANT_TERMINAL "xPing with terminal" ON)
  if(X_ASSISTANT_TERMINAL)
    set_target_properties(xPing PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
  else()
    # QDebug info will be printed to terminal(for vscode)
  endif()
endif()

# --------------------------------------------------------------------------------------------------
# Make installer for Windows
if(WIN32 AND X_TOOLS_LATEST_GIT_TAG)
  set(icon ${CMAKE_CURRENT_LIST_DIR}/xPing.ico)
  x_tools_generate_zip(xPing ${X_TOOLS_LATEST_GIT_TAG})
  x_tools_generate_msix(xPing "xPing" "xPing" ${X_TOOLS_LATEST_GIT_TAG} FALSE)
  x_tools_generate_installer(xPing ${X_TOOLS_LATEST_GIT_TAG} ${icon})
endif()
