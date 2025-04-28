if(QT_VERSION VERSION_LESS 6.8.0)
  function(x_setup_mqtt target)
    # No-op for Qt versions less than 6.8.0
    return()
  endfunction()

  return()
endif()

add_compile_definitions(X_ENABLE_MQTT)
set(MQTT_ZIP qtmqtt-6.8.3)
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd/${MQTT_ZIP})
  set(MQTT_URL https://codeload.github.com/qt/qtmqtt/zip/refs/tags/v6.8.3)
  file(DOWNLOAD ${MQTT_URL} ${CMAKE_SOURCE_DIR}/3rd/${MQTT_ZIP}.zip SHOW_PROGRESS)

  list(GET DOWNLOAD_STATUS 0 DOWNLOAD_RESULT)
  if(NOT DOWNLOAD_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to download ${MQTT_ZIP}.zip.")
  endif()

  message(STATUS "Extracting ${MQTT_ZIP}.zip...")
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/3rd/${MQTT_ZIP}.zip
                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
endif()

add_subdirectory(${CMAKE_SOURCE_DIR}/3rd/${MQTT_ZIP})

function(x_setup_mqtt target)
  if(QT_VERSION VERSION_LESS 6.8.0)
    return()
  endif()
  target_link_libraries(${target} PRIVATE Qt6::Mqtt)
endfunction()
