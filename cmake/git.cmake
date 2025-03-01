# --------------------------------------------------------------------------------------------------
# Get the last tag.
function(x_tools_git_get_latest_tag working_dir prefix)
  execute_process(
    COMMAND git describe --abbrev=0 --tags
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_LATEST_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  set(USING_DATE_TIME FALSE)
  if(NOT GIT_LATEST_TAG)
    set(USING_DATE_TIME TRUE)
  else()
    if(${GIT_LATEST_TAG} STREQUAL "")
      set(USING_DATE_TIME TRUE)
    elseif(${GIT_LATEST_TAG} STREQUAL "continuous")
      set(USING_DATE_TIME TRUE)
    endif()
  endif()

  if(USING_DATE_TIME)
    string(TIMESTAMP current_year "%Y")
    string(TIMESTAMP current_month "%m")
    string(TIMESTAMP current_day "%d")
    math(EXPR current_month "${current_month} + 0")
    math(EXPR current_day "${current_day} + 0")
    set(target_version "v${current_year}.${current_month}.${current_day}")
    set(GIT_LATEST_TAG ${target_version})
  endif()

  message(STATUS "[git] Latest git tag(${prefix}_LATEST_GIT_TAG): ${GIT_LATEST_TAG}")
  set(${prefix}_LATEST_GIT_TAG="${GIT_LATEST_TAG}" CACHE STRING "Latest git tag" FORCE)
  set(${prefix}_LATEST_GIT_TAG
      "${GIT_LATEST_TAG}"
      PARENT_SCOPE)
  add_compile_definitions(${prefix}_LATEST_GIT_TAG="${GIT_LATEST_TAG}")
endfunction()

# --------------------------------------------------------------------------------------------------
# Get the last commit.
function(x_tools_git_get_latest_commit working_dir prefix)
  execute_process(
    COMMAND git log -1 --pretty=%H
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "[git] Latest git commit(${prefix}_GIT_COMMIT): ${GIT_COMMIT}")
  add_compile_definitions(${prefix}_GIT_COMMIT="${GIT_COMMIT}")
endfunction()

# --------------------------------------------------------------------------------------------------
# Get last commit time.
function(x_tools_git_get_latest_commit_time working_dir prefix)
  execute_process(
    COMMAND git log -1 --format=%cd
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "[git] Latest git commit time(${prefix}_GIT_COMMIT_TIME): ${GIT_COMMIT_TIME}")
  add_compile_definitions(${prefix}_GIT_COMMIT_TIME="${GIT_COMMIT_TIME}")
endfunction()
