find_package(Qt6 REQUIRED COMPONENTS Qml Quick QuickControls2)

file(GLOB X_PING_QML ${CMAKE_CURRENT_SOURCE_DIR}/Qml/*.*)
file(GLOB X_PING_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.*)
list(APPEND X_PING_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/xPing.rc)
list(APPEND X_PING_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/xPing.qrc)
list(APPEND X_PING_SOURCE ${X_PING_QML})

x_tools_add_executable(xPing ${X_PING_SOURCE})
x_tools_deploy_qt(xPing)
x_tools_tar_target(xPing)
x_tools_copy_microsoft_store_dll(xPing)
x_tools_get_latest_version_from_tag(xPing X_PING_VERSION)
x_tools_generate_translations(xPing)

target_link_libraries(xPing PUBLIC xApp Qt6::Qml Qt6::Quick Qt6::QuickControls2)

# --------------------------------------------------------------------------------------------------
# Generate installer(exe)
set(root_dir "${CMAKE_BINARY_DIR}/installer/xPing")
set(icon ${CMAKE_CURRENT_SOURCE_DIR}/xPing.ico)
x_tools_generate_installer(xPing xPing ${X_PING_VERSION})

# --------------------------------------------------------------------------------------------------
# Generate Microsoft Store package(MSIX)
if(WIN32)
  x_tools_generate_msix(xPing "PingPing" "Ping Ping" "${X_PING_VERSION}" OFF)
endif()

set_target_properties(xPing PROPERTIES FOLDER "xPing")
set_target_properties(xPing_lupdate PROPERTIES FOLDER "xPing")
set_target_properties(xPing_lrelease PROPERTIES FOLDER "xPing")
set_target_properties(xPing_lupgrade PROPERTIES FOLDER "xPing")
set_target_properties(xPing_installer PROPERTIES FOLDER "xPing")
set_target_properties(xPing_msix PROPERTIES FOLDER "xPing")
