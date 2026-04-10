file(GLOB X_TFTP_COMMON_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/tftp/common/*.*")
list(APPEND X_TFTP_COMMON_FILES ${X_COMMON_FILES})

include(cmake/x_qt_deploy.cmake)
include(cmake/x_qt_linguist.cmake)
include(cmake/msix/msix.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/client/xTftpClient.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/server/xTftpServer.cmake)
