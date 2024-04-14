add_compile_definitions(X_TOOLS_AUTHOR="x-tools-author")
add_compile_definitions(X_TOOLS_EDITION="beta1")
add_compile_definitions(X_TOOLS_VERSION="6.0.0")
add_compile_definitions(X_TOOLS_DEFAULT_APP_STYLE="Fusion")
add_compile_definitions(X_TOOLS_CLEAR_MESSAGE_INTERVAL=8000)
add_compile_definitions(X_TOOLS_AUTHOR_EMAIL="x-tools@outlook.com")
add_compile_definitions(X_TOOLS_GITEE_REPOSITORY_URL="https://gitee.com/x-tools-author/x-tools")
add_compile_definitions(X_TOOLS_GITHUB_REPOSITORY_URL="https://github.com/x-tools-author/x-tools")

set(X_TOOLS_BINARY_DIR ${CMAKE_BINARY_DIR}/assets)

# Set the suffix of the library.
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  set(X_TOOLS_FILE_SUFFIX "")
else()
  set(X_TOOLS_FILE_SUFFIX "d")
endif()

# Unzip file and import it as a sub module.
function(x_tools_add_third_party zip_file_name_without_suffix)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${zip_file_name_without_suffix}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/${zip_file_name_without_suffix})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/${zip_file_name_without_suffix})
endfunction()

function(x_tools_copy_glog target)
  if(${BUILD_SHARED_LIBS})
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glog::glog>
              "${X_TOOLS_BINARY_DIR}/${target}/$<TARGET_FILE_NAME:glog::glog>")
  endif()
endfunction()

# Add executable. It can be used by Qt5 and Qt6.
function(x_tools_add_executable target sources)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${X_TOOLS_BINARY_DIR}/${target}")
  if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${target} MANUAL_FINALIZATION)
    if(${target} STREQUAL "xTools")
      set_property(
        TARGET ${target}
        APPEND
        PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)
    endif()
  else()
    if(ANDROID)
      add_library(${target} SHARED)
    else()
      add_executable(${target})
    endif()
  endif()

  set(INDEX 0)
  while(INDEX LESS ${ARGC})
    math(EXPR INDEX "${INDEX} + 1")
    target_sources(${target} PRIVATE ${ARGV${INDEX}})
  endwhile()

  if(ANDROID)
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${target}>"
              "${CMAKE_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/$<TARGET_FILE_NAME:${target}>")
  endif()

  x_tools_copy_glog(${target})
  x_tools_set_target_properties(${target})

  if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${target})
  endif()
endfunction()

function(x_tools_set_target_properties target)
  if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set_target_properties(
      ${target}
      PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
                 ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
                 ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
                 TRUE WIN32_EXECUTABLE
                 TRUE)
  else()
    set_target_properties(
      ${target}
      PROPERTIES ${BUNDLE_ID_OPTION} MACOSX_BUNDLE_BUNDLE_VERSION
                 ${PROJECT_VERSION} MACOSX_BUNDLE_SHORT_VERSION_STRING
                 ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} MACOSX_BUNDLE
                 TRUE)
  endif()
endfunction()

function(x_tools_tar_target target)
  if(WIN32)
    string(TOLOWER ${target} lower_target)
    string(TOLOWER ${CMAKE_HOST_SYSTEM_NAME} lower_system_name)
    string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} lower_system_processor)
    set(TAR_FILE_NAME ${lower_target}-${lower_system_name}-${lower_system_processor})
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E tar "cf" ${TAR_FILE_NAME}.zip "--format=zip" ${target}
      WORKING_DIRECTORY ${X_TOOLS_BINARY_DIR})
  endif()
endfunction()

function(x_tools_finalize_executable target)
  if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${target})
  endif()
endfunction()

function(x_tools_generate_translations target)
  if(QT_VERSION_MAJOR LESS 6)
    return()
  endif()

  set(APP_TS_FILES "")
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Translations/${target}_en.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Translations/${target}_zh_CN.ts)
  set(O_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Translations")
  if(NOT QT_VERSION VERSION_LESS "6.7.0")
    qt_add_lupdate(
      SOURCE_TARGETS
      ${target}
      TS_FILES
      ${APP_TS_FILES}
      LUPDATE_TARGET
      ${target}_lupdate
      NO_GLOBAL_TARGET)
  else()
    qt_add_lupdate(${target} TS_FILES ${APP_TS_FILES})
  endif()

  set_source_files_properties(${APP_TS_FILES} PROPERTIES OUTPUT_LOCATION ${O_PATH})
  if(NOT QT_VERSION VERSION_LESS "6.7.0")
    qt_add_lrelease(TS_FILES ${APP_TS_FILES} LRELEASE_TARGET ${target}_lrelease NO_GLOBAL_TARGET)
  else()
    qt_add_lrelease(${target} TS_FILES ${APP_TS_FILES})
  endif()
endfunction()

set(QT_IFW_VERSION "4.6")
if(WIN32)
  set(binarycreator_temp "Tools/QtInstallerFramework/${QT_IFW_VERSION}/bin/binarycreator.exe")
  set(binarycreator ${QT_DIR}/../../../../../${binarycreator_temp})
  set(binarysuffix ".exe")
elseif(UNIX AND NOT APPLE)
  set(binarycreator_temp "Tools/QtInstallerFramework/${QT_IFW_VERSION}/bin/binarycreator")
  set(binarycreator ${QT_DIR}/../../../../../${binarycreator_temp})
  set(binarysuffix ".run")
endif()

function(x_tools_generate_installer_with_qt_ifw target root_dir version icon)
  string(TOLOWER ${target} target_lower)
  add_custom_target(
    ${target}Installer
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${root_dir}
    COMMAND ${CMAKE_COMMAND} -DTARGET=${target} -DROOT_DIR=${root_dir} -DVERSION=${version}
            -DICON=${icon} -P ${CMAKE_SOURCE_DIR}/CMake/xToolsScriptGenerateQIFW.cmake
    COMMAND ${CMAKE_COMMAND} -E echo ${icon}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${icon}
            ${root_dir}/packages/${target}/data/icon.ico
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            ${root_dir}/packages/${target}/data
    COMMAND ${binarycreator} --offline-only -c ${root_dir}/config/config.xml -p ${root_dir}/packages
            ${root_dir}/../${target_lower}-${version}-installer${binarysuffix}
    SOURCES ${CMAKE_SOURCE_DIR}/.cmake/sak_installer.cmake
    COMMENT "Start making installer(${target})...")
endfunction()
