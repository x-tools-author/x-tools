# * https://github.com/obgm/libcoap
return()
if(${X_DOWNLOAD_FROM_GITEE})

else()

endif()
FetchContent_Declare(
  libcoap
  GIT_REPOSITORY https://gitee.com/mirrors/libcoap.git
  GIT_TAG v4.3.5a)
FetchContent_MakeAvailable(libcoap)
