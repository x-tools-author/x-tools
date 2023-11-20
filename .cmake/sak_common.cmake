# Set the suffix of the library.
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")

else()
  set(SAK_FILE_SUFFIX "d")
endif()

# Get the last commit.
function(sak_get_last_commit working_dir prefix)
  execute_process(
    COMMAND git log -1 --pretty=%H
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  add_compile_definitions(${prefix}_GIT_COMMIT="${GIT_COMMIT}")
endfunction()

# Get last commit time.
function(sak_get_last_commit_time working_dir prefix)
  execute_process(
    COMMAND git log -1 --format=%cd
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  add_compile_definitions(${prefix}_GIT_COMMIT_TIME="${GIT_COMMIT_TIME}")
endfunction()

# Unzip file and imort it as a sub module.
function(sak_add_subdiretory zip_file_name_without_suffix)
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${zip_file_name_without_suffix}.zip
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd)
  add_subdirectory(3rd/${zip_file_name_without_suffix})
endfunction()

# Find Qt package.
macro(sak_find_qt_package modules)
  find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${modules})
  find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${modules})
endmacro()
