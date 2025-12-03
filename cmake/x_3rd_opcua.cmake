# * https://github.com/open62541/open62541
return()
if(${X_DOWNLOAD_FROM_GITEE})
  FetchContent_Declare(
    open62541
    GIT_REPOSITORY https://gitee.com/mirrors/open62541.git
    GIT_TAG v1.4.14)
else()
  FetchContent_Declare(
    open62541
    GIT_REPOSITORY https://github.com/open62541/open62541.git
    GIT_TAG v1.4.14)
endif()
FetchContent_MakeAvailable(open62541)
