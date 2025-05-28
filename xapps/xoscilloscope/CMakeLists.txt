find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets SerialPort PrintSupport)

file(GLOB_RECURSE SERIAL_PORTOSCILLOSCOPE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.*)
list(APPEND SERIAL_PORTOSCILLOSCOPE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/SerialPortOscilloscope.qrc)
list(APPEND SERIAL_PORTOSCILLOSCOPE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/SerialPortOscilloscope.rc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Source)

x_tools_add_executable(SerialPortOscilloscope ${SERIAL_PORTOSCILLOSCOPE_SOURCE})
x_tools_deploy_qt(SerialPortOscilloscope)
x_tools_generate_translations(SerialPortOscilloscope)
target_link_libraries(SerialPortOscilloscope PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets
                                                     Qt6::SerialPort Qt6::PrintSupport)

# --------------------------------------------------------------------------------------------------
# Generate Microsoft Store package(MSIX)
if(WIN32)
  x_tools_generate_msix(SerialPortOscilloscope "SerialPortOscilloscope" "Serial Port Oscilloscope"
                        "1.0.0" OFF)
endif()

# --------------------------------------------------------------------------------------------------
# The floder of the project
set_target_properties(SerialPortOscilloscope PROPERTIES FOLDER "xCharts")
set_target_properties(SerialPortOscilloscope_lupdate PROPERTIES FOLDER "xCharts")
set_target_properties(SerialPortOscilloscope_lrelease PROPERTIES FOLDER "xCharts")
set_target_properties(SerialPortOscilloscope_lupgrade PROPERTIES FOLDER "xCharts")
set_target_properties(SerialPortOscilloscope_msix PROPERTIES FOLDER "xCharts")