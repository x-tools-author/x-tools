find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets SerialPort PrintSupport)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src/*.*)
list(APPEND SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/xOscilloscope.qrc)
list(APPEND SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/xOscilloscope.rc)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

include(cmake/x_qt_deploy.cmake)
include(cmake/x_qt_linguist.cmake)

qt_add_executable(xOscilloscope ${SRC_FILES})
x_deploy_qt(xOscilloscope)
target_link_libraries(xOscilloscope PRIVATE Qt::Core Qt::Gui Qt::Widgets Qt::SerialPort
                                            Qt::PrintSupport)
# --------------------------------------------------------------------------------------------------
# Generate Microsoft Store package(MSIX)
if(WIN32)
  # x_generate_msix(SerialPortOscilloscope "SerialPortOscilloscope" "Serial Port Oscilloscope"
  # "1.1.0" OFF)
endif()
