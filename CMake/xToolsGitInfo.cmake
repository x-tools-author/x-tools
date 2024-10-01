# Get the last tag.
function(x_tools_git_get_latest_tag working_dir prefix)
  execute_process(
    COMMAND git tag
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE git_tags
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  string(REPLACE "\r" "" git_tags ${git_tags})
  string(REPLACE "\n" ";" git_tags ${git_tags})
  list(LENGTH git_tags git_tags_count)
  list(GET git_tags -1 GIT_LATEST_TAG)
  if(${GIT_LATEST_TAG} STREQUAL "" OR ${GIT_LATEST_TAG} STREQUAL "continuous")
    string(TIMESTAMP target_version "%y.%m.%d")
    set(GIT_LATEST_TAG ${target_version})
  endif()
  message("Latest git tag: ${GIT_LATEST_TAG}")
  set(${prefix}_GIT_TAG="${GIT_LATEST_TAG}" CACHE STRING "Latest git tag" FORCE)
  set(GIT_TAG
      "${GIT_LATEST_TAG}"
      PARENT_SCOPE)
  add_compile_definitions(${prefix}_GIT_TAG="${GIT_LATEST_TAG}")
endfunction()

# Get the last commit.
function(x_tools_git_get_latest_commit working_dir prefix)
  execute_process(
    COMMAND git log -1 --pretty=%H
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message("Latest git commit(${prefix}_GIT_COMMIT): ${GIT_COMMIT}")
  add_compile_definitions(${prefix}_GIT_COMMIT="${GIT_COMMIT}")
endfunction()

# Get last commit time.
function(x_tools_git_get_latest_commit_time working_dir prefix)
  execute_process(
    COMMAND git log -1 --format=%cd
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_COMMIT_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message("Latest git commit time: ${GIT_COMMIT_TIME}")
  add_compile_definitions(${prefix}_GIT_COMMIT_TIME="${GIT_COMMIT_TIME}")
endfunction()
