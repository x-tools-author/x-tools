# * https://github.com/obgm/libcoap
return()
if(${X_DOWNLOAD_FROM_GITEE})
  FetchContent_Declare(
    libcoap
    GIT_REPOSITORY https://gitee.com/mirrors/libcoap.git
    GIT_TAG v4.3.5a)
else()
  FetchContent_Declare(
    libcoap
    GIT_REPOSITORY https://github.com/obgm/libcoap.git
    GIT_TAG v4.3.5a)
endif()
FetchContent_MakeAvailable(libcoap)
