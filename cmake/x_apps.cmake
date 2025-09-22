macro(remove_all_files_of_app dir_name)
  message(STATUS "[xTools-${dir_name}] Remove all files of ${dir_name}")
  file(GLOB_RECURSE app_files "${CMAKE_SOURCE_DIR}/src/${dir_name}/*.*")
  foreach(file ${app_files})
    list(REMOVE_ITEM X_SOURCES ${file})
  endforeach()
endmacro()

macro(setup_x_app dir_name default_enabled)
  string(TOUPPER ${dir_name} dir_name_upper)
  option(X_ENABLE_X_${dir_name_upper} "Enable ${dir_name} support" ${default_enabled})

  if(IOS OR ANDROID)
    set(X_ENABLE_X_${dir_name_upper}
        OFF
        CACHE BOOL "Disable ${dir_name} support on mobile" FORCE)
  endif()

  if(QT_VERSION VERSION_LESS "6.8.0")
    set(X_ENABLE_X_${dir_name_upper}
        OFF
        CACHE BOOL "Disable ${dir_name} support on Qt<6.8.0" FORCE)
  endif()

  if(X_ENABLE_X_${dir_name_upper})
    include_directories(${CMAKE_SOURCE_DIR}/src/${dir_name})
    add_compile_definitions(X_ENABLE_X_${dir_name_upper}=1)
  else()
    remove_all_files_of_app(${dir_name})
    add_compile_definitions(X_ENABLE_X_${dir_name_upper}=0)
  endif()
endmacro()

setup_x_app(mqtt OFF)
setup_x_app(canbus OFF)
setup_x_app(modbus OFF)
