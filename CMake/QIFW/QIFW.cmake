set(X_TOOLS_QIFW_LATEST_VERSION "none")
if(EXISTS ${QT_DIR}/../../../../../Tools/QtInstallerFramework)
  file(GLOB all_sub_dirs_tmp ${QT_DIR}/../../../../../Tools/QtInstallerFramework/*)
  foreach(dir ${all_sub_dirs_tmp})
    if(IS_DIRECTORY ${dir})
      file(RELATIVE_PATH relative_dir ${QT_DIR}/../../../../../Tools/QtInstallerFramework ${dir})
      list(APPEND version_list ${relative_dir})
      set(X_TOOLS_QIFW_LATEST_VERSION ${relative_dir})
    endif()
  endforeach()

  message(STATUS "version_list:${version_list}")
  set(X_TOOLS_QIFW_VERSION
      ${X_TOOLS_QIFW_LATEST_VERSION}
      CACHE STRING "Using the version of Qt Installer Framework")
  set_property(CACHE X_TOOLS_QIFW_VERSION PROPERTY STRINGS ${version_list})
endif()

if(WIN32)
  set(temp Tools/QtInstallerFramework/${X_TOOLS_QIFW_VERSION}/bin/binarycreator.exe)
  set(binarycreator ${QT_DIR}/../../../../../${temp})
elseif(UNIX AND NOT APPLE)
  set(temp Tools/QtInstallerFramework/${X_TOOLS_QIFW_VERSION}/bin/binarycreator)
  set(binarycreator ${QT_DIR}/../../../../../${temp})
endif()

function(x_tools_generate_installer target version icon)
  if(APPLE)
    return()
  endif()

  set(args "-DargTarget=${target}")
  list(APPEND args "-DargRootDir=${X_TOOLS_BINARY_DIR}/Installer/${target}")
  list(APPEND args "-DargVersion=${version}")
  list(APPEND args "-DargDataDir=$<TARGET_FILE_DIR:${target}>")
  list(APPEND args "-DargTemperateDir=${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Installer")
  list(APPEND args "-DargBinarycreator=${binarycreator}")
  list(APPEND args "-DargIcon=${icon}")

  set(RESOURCES_DIR ${CMAKE_CURRENT_FUNCTION_LIST_DIR})
  set(QIFW_SOURCE ${RESOURCES_DIR}/QIFW.cmake)
  list(APPEND QIFW_SOURCE ${RESOURCES_DIR}/QIFWScript.cmake)
  list(APPEND QIFW_SOURCE ${RESOURCES_DIR}/Installer/config/config.xml)
  list(APPEND QIFW_SOURCE ${RESOURCES_DIR}/Installer/packages/all/meta/package.xml)
  list(APPEND QIFW_SOURCE ${RESOURCES_DIR}/Installer/packages/all/meta/installscript.js)

  add_custom_target(
    ${target}_installer
    COMMAND ${CMAKE_COMMAND} ${args} -P ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/QIFWScript.cmake
    SOURCES ${QIFW_SOURCE}
    COMMENT "Start making installer packet for ${target}")
endfunction()
