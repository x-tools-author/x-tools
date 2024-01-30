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

function(sak_generate_installer_with_qt_ifw target root_dir version icon)
  string(TOLOWER ${target} target_lower)
  add_custom_target(
    ${target}-Installer
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${root_dir}
    COMMAND ${CMAKE_COMMAND} -DTARGET=${target} -DROOT_DIR=${root_dir} -DVERSION=${version}
            -DICON=${icon} -P ${CMAKE_SOURCE_DIR}/.cmake/sak_script_generate_qtifw_resources.cmake
    COMMAND ${CMAKE_COMMAND} -E echo ${icon}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${icon}
            ${root_dir}/packages/${target}/data/icon.ico
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            ${root_dir}/packages/${target}/data
    COMMAND ${binarycreator} --offline-only -c ${root_dir}/config/config.xml -p ${root_dir}/packages
            ${root_dir}/../${target_lower}-${version}-installer${binarysuffix}
    SOURCES ${RES_FILES}
    COMMENT "Start making installer(${target})...")
endfunction()
