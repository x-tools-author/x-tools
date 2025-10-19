file(GLOB_RECURSE x_common ${CMAKE_SOURCE_DIR}/xapps/x/*.*)
file(GLOB_RECURSE x_linguist_src ${CMAKE_SOURCE_DIR}/xapps/xlinguist/src/*.*)
file(GLOB_RECURSE x_tools_common ${CMAKE_SOURCE_DIR}/src/common/*.*)

include_directories(${CMAKE_SOURCE_DIR}/xapps)
include_directories(${CMAKE_SOURCE_DIR}/xapps/xlinguist/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${X_BINS_DIR}/xLinguist)
qt_add_executable(xLinguist ${x_common} ${x_linguist_src} ${x_tools_common}
                  ${CMAKE_CURRENT_LIST_DIR}/xLinguist.qrc)
target_link_libraries(xLinguist PRIVATE Qt::Widgets Qt::Network Qt::Svg Qt::SerialPort ${X_LIBS})
# set_target_properties(xLinguist PROPERTIES WIN32_EXECUTABLE TRUE MACOSX_BUNDLE TRUE)
x_deploy_qt(xLinguist)
x_generate_translations(xLinguist)

if(WIN32)
  if(MSVC)
    target_link_libraries(xLinguist PRIVATE Dwmapi)
  endif()
endif()
