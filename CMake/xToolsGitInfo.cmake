# Get the last tag.
set(GIT_TAG "x.x.x")
function(x_tools_git_get_latest_tag working_dir prefix)
  execute_process(
    COMMAND git describe --abbrev=0
    WORKING_DIRECTORY ${working_dir}
    OUTPUT_VARIABLE GIT_LATEST_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message("Latest git tag: ${GIT_LATEST_TAG}")
  set(${prefix}_GIT_TAG="${GIT_LATEST_TAG}")
  set(GIT_TAG "${GIT_LATEST_TAG}")
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
