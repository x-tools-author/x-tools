include(${CMAKE_SOURCE_DIR}/cmake/x_qt_deploy.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_linguist.cmake)

# include(FetchContent)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_charts.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_serialbus.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_serialport.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_bluetooth.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_websockets.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/x_qt_location.cmake)

# --------------------------------------------------------------------------------------------------
macro(x_add_qt_module module_name)
  set(dst_dir ${X_LIBS_DIR}/${module_name}-${QT_VERSION})
  FetchContent_Declare(
    ${module_name}
    GIT_REPOSITORY https://github.com/qt/${module_name}.git
    GIT_TAG ${QT_VERSION})
  FetchContent_MakeAvailable(${module_name})

  add_custom_target(
    ${module_name}_deploy
    COMMAND ${CMAKE_COMMAND} --install . --prefix ${dst_dir}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/${module_name}-build
    COMMENT "Installing ${module_name} to ${dst_dir}")
  set_property(TARGET ${module_name}_deploy PROPERTY FOLDER "QtModules")
endmacro()
