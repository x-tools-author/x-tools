file(GLOB x_files "${CMAKE_SOURCE_DIR}/xapps/x/*.*")
file(GLOB x_utilities "${CMAKE_SOURCE_DIR}/src/utilities/*.*")
set(X_COMMON_FILES ${x_files} ${x_utilities})

if(QT_VERSION VERSION_LESS "6.8.0")
  return()
endif()

set(X_APPS "xTools")
list(APPEND X_APPS "xSvg")
list(APPEND X_APPS "xCode")
list(APPEND X_APPS "xDemo")
list(APPEND X_APPS "xHash")
list(APPEND X_APPS "xPing")
list(APPEND X_APPS "xDebug")
list(APPEND X_APPS "xAssistant")
list(APPEND X_APPS "xOscilloscope")

if(EXISTS ${CMAKE_SOURCE_DIR}/xapps/xprivate)
  # Get all directories in xapps/xprivate
  file(GLOB PRIVATE_APPS "${CMAKE_SOURCE_DIR}/xapps/xprivate/*")
  foreach(private_app ${PRIVATE_APPS})
    if(IS_DIRECTORY ${private_app})
      get_filename_component(app_name ${private_app} NAME)
      # * The second char of the app name should be uppercase
      # * if app_name == xlinguist, the char2 is 'l', it will be converted to 'L'
      # * if app_name == xlinguist, the rest_of_name is 'inguist'
      # * if app_name == xlinguist, the app_name will be 'xLinguist'(x + L + inguist)
      string(SUBSTRING ${app_name} 1 1 char2)
      string(TOUPPER ${char2} char2)
      string(SUBSTRING "${app_name}" 2 -1 rest_of_name)
      string(CONCAT app_name "x${char2}${rest_of_name}")
      list(APPEND X_APPS "${app_name}")
    endif()
  endforeach()
endif()

# cmake-format: off
set(X_APP "xTools" CACHE STRING "Select a x-app to build")
set_property(CACHE X_APP PROPERTY STRINGS ${X_APPS})
string(TOLOWER ${X_APP} LOWER_X_APP)
if(EXISTS ${CMAKE_SOURCE_DIR}/xapps/${LOWER_X_APP})
  include_directories(${CMAKE_SOURCE_DIR}/xapps)
  include(${CMAKE_SOURCE_DIR}/xapps/${LOWER_X_APP}/${X_APP}.cmake)
elseif(EXISTS ${CMAKE_SOURCE_DIR}/xapps/xprivate/${LOWER_X_APP})
  include_directories(${CMAKE_SOURCE_DIR}/xapps)
  include(${CMAKE_SOURCE_DIR}/xapps/xprivate/${LOWER_X_APP}/${X_APP}.cmake)
endif()
# cmake-format: on
