option(X_ENABLE_xApps "Enable xFlow support" OFF)
if(NOT X_ENABLE_xApps)
  return()
endif()

add_compile_definitions(X_ENABLE_xApps)
include(${CMAKE_CURRENT_LIST_DIR}/x_apps_modbus.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/x_apps_mqtt.cmake)
