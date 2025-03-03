add_compile_definitions(X_TOOLS_AUTHOR="x-tools-author")
add_compile_definitions(X_TOOLS_AUTHOR_EMAIL="x-tools@outlook.com")
add_compile_definitions(X_TOOLS_GITEE_REPOSITORY_URL="https://gitee.com/x-tools-author/x-tools")
add_compile_definitions(X_TOOLS_GITHUB_REPOSITORY_URL="https://github.com/x-tools-author/x-tools")

# Unzip file and import it as a sub module.
function(x_tools_add_third_party zip_file_name_without_suffix)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${zip_file_name_without_suffix}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/${zip_file_name_without_suffix})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd/${zip_file_name_without_suffix})
endfunction()

function(x_tools_copy_glog target)
  if(${BUILD_SHARED_LIBS})
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glog::glog>
              "${CMAKE_BINARY_DIR}/${target}/$<TARGET_FILE_NAME:glog::glog>")
  endif()
endfunction()

# Add executable. It can be used by Qt5 and Qt6.
function(x_tools_add_executable target)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${target}")
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
      add_library(${target} SHARED ${ARGS})
    else()
      add_executable(${target} ${ARGS})
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

  set_target_properties(
    ${target}
    PROPERTIES # MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appuntitled3
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE)
endfunction()

function(x_tools_generate_zip target version)
  if(WIN32)
    string(TOLOWER ${target} lower_target)
    string(TOLOWER ${CMAKE_HOST_SYSTEM_NAME} lower_system_name)
    string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} lower_system_processor)
    set(TAR_FILE_NAME ${lower_target}-${lower_system_name}-${lower_system_processor}-v${version})

    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${target}-zip
                    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../)

    add_custom_target(
      ${target}-zip
      COMMAND ${CMAKE_COMMAND} -E tar "cf" ${target}-zip/${TAR_FILE_NAME}.zip "--format=zip"
              ${target}
      WORKING_DIRECTORY "$<TARGET_FILE_DIR:${target}>/../"
      SOURCES cmake/xtools.cmake)
  endif()
endfunction()

function(x_tools_generate_translations target)
  if(QT_VERSION VERSION_LESS "6.2.0")
    return()
  endif()

  set(APP_TS_FILES "")
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/translations/${target}_en.ts)
  list(APPEND APP_TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/translations/${target}_zh_CN.ts)
  # cmake-format: off
  if(NOT QT_VERSION VERSION_LESS "6.7.0")
    qt_add_lupdate(SOURCE_TARGETS ${target} TS_FILES ${APP_TS_FILES} LUPDATE_TARGET ${target}_lupdate NO_GLOBAL_TARGET)
  else()
    set(O_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res/translations")
    qt_add_lupdate(${target} TS_FILES ${APP_TS_FILES})
  endif()
  # cmake-format: on

  set_source_files_properties(${APP_TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR})
  if(NOT QT_VERSION VERSION_LESS "6.7.0")
    qt_add_lrelease(TS_FILES ${APP_TS_FILES} LRELEASE_TARGET ${target}_lrelease NO_GLOBAL_TARGET)
  else()
    qt_add_lrelease(${target} TS_FILES ${APP_TS_FILES})
  endif()

  add_custom_target(
    ${target}_lupgrade
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/${target}_en.qm
            ${CMAKE_CURRENT_SOURCE_DIR}/res/translations/${target}_en.qm
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/${target}_zh_CN.qm
            ${CMAKE_CURRENT_SOURCE_DIR}/res/translations/${target}_zh_CN.qm
    DEPENDS ${target}_lrelease
    COMMENT "Generate translations for ${target}...")
endfunction()
