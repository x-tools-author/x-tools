option(X_ENABLE_X_APPS "Enable xApps" OFF)

if(IOS OR ANDROID)
  set(X_ENABLE_X_APPS
      OFF
      CACHE BOOL "Disable xApps on mobile platforms" FORCE)
endif()

if(QT_VERSION VERSION_LESS "6.8.0")
  set(X_ENABLE_X_APPS
      OFF
      CACHE BOOL "Disable xApps on Qt < 6.8.0" FORCE)
endif()

macro(remove_all_files_of_app dir_name)
  message(STATUS "[xTools-${dir_name}] Remove all files of ${dir_name}")
  file(GLOB_RECURSE app_files "${CMAKE_SOURCE_DIR}/src/${dir_name}/*.*")
  foreach(file ${app_files})
    list(REMOVE_ITEM X_SOURCES ${file})
  endforeach()
endmacro()

macro(setup_x_app dir_name)
  if(NOT X_ENABLE_X_APPS)
    remove_all_files_of_app(${dir_name})
  else()
    option(X_ENABLE_X_${dir_name} "Enable ${dir_name} support" ON)
    if(NOT X_ENABLE_X_${dir_name})
      remove_all_files_of_app(${dir_name})
    else()
      include_directories(${CMAKE_SOURCE_DIR}/src/${dir_name})
      string(TOUPPER ${dir_name} dir_name_upper)
      add_compile_definitions(X_ENABLE_X_${dir_name_upper})
    endif()
  endif()
endmacro()

setup_x_app(mqtt)
setup_x_app(canbus)
setup_x_app(modbus)
