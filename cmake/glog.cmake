# https://github.com/google/glog
set(glog_package_name "glog-0.7.1")

# --------------------------------------------------------------------------------------------------
# Extract glog zip...
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${glog_package_name})
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${glog_package_name}.zip
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
endif()

# --------------------------------------------------------------------------------------------------
# Add glog module...
set(glog_dst_dir ${X_DEPLOY_LIBS_DIR}/${glog_package_name})
if(EXISTS ${glog_dst_dir})
  set(CMAKE_PREFIX_PATH ${glog_dst_dir} ${CMAKE_PREFIX_PATH})
  find_package(glog REQUIRED)
else()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/${glog_package_name})
  set_property(TARGET glog PROPERTY FOLDER "3rd")
  set_property(TARGET glog_internal PROPERTY FOLDER "3rd")
  x_install_3rd_library(glog ${glog_package_name})
endif()
