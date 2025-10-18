# * https://github.com/itay-grudev/SingleApplication
# * https://codeload.github.com/itay-grudev/SingleApplication/zip/refs/tags/v3.5.2
set(package_version "3.5.2")
set(package_name "SingleApplication-${package_version}")
set(tmp "https://codeload.github.com/itay-grudev/SingleApplication/zip/refs/tags")
set(package_url "${tmp}/v${package_version}")

add_compile_definitions(X_ENABLE_SINGLE_APPLICATION=0)
return()

if(ANDROID OR IOS)
  message(STATUS "[SingleApplication]SingleApplication is not supported on Android or iOS.")
  return()
endif()

if(NOT WIN32)
  message(STATUS "[SingleApplication]SingleApplication is not supported on this platform.")
  return()
endif()

if(QT_VERSION VERSION_LESS "6.8.0")
  message(STATUS "[SingleApplication]SingleApplication requires Qt 6.8 or higher.")
  return()
endif()

# Download the package
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip")
  file(
    DOWNLOAD "${package_url}" "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip"
    SHOW_PROGRESS
    STATUS download_status)
  if(NOT download_status EQUAL 0)
    message(WARNING "[SingleApplication]Failed to download SingleApplication.")
    return()
  endif()
endif()

# Unzip the package
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}")
  file(ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}.zip" DESTINATION
       "${CMAKE_CURRENT_SOURCE_DIR}/3rd")
endif()

if(MSVC)
  set(sapp_lib_file ${X_LIBS_DIR}/${package_name}/lib/SingleApplication.lib)
else()
  set(sapp_lib_file ${X_LIBS_DIR}/${package_name}/lib/SingleApplication.a)
endif()

get_filename_component(sapp_lib_dir ${sapp_lib_file} DIRECTORY)
set(QAPPLICATION_CLASS QApplication)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rd/${package_name}")
set_property(TARGET SingleApplication PROPERTY FOLDER "3rd")
set_target_properties(SingleApplication PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${sapp_lib_dir})

list(APPEND X_LIBS SingleApplication)
add_compile_definitions(X_ENABLE_SINGLE_APPLICATION=1)
set(X_ENABLE_SINGLE_APPLICATION ON)
