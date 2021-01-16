# Interval of timer which is used to clear ui message
DEFINES += SAK_CLEAR_MESSAGE_INTERVAL=8000
# The max count of auto response item
DEFINES += SAK_MAX_AUTO_RESPONSE_COUNT=9999
# All address of host
DEFINES += SAK_HOST_ADDRESS_ANY=\"\\\"HostAddress::Any\\\"\"
# Exit code which is used to reboot the application
DEFINES += SAK_REBOOT_CODE=1314
# Sleep interval of device thread, the unit is ms
DEFINES += SAK_DEVICE_THREAD_SLEEP_INTERVAL=10
# Github repositories
DEFINES += SAK_GITHUB_REPOSITORY_URL=\"\\\"https://github.com/qsaker/QtSwissArmyKnife\\\"\"
# Gitee repositories
DEFINES += SAK_GITEE_REPOSITORY_URL=\"\\\"https://gitee.com/qsaker/QtSwissArmyKnife\\\"\"
# The default application style of Windows and Unix
win32|unix {
    DEFINES += SAK_STYLE_DEFAULT=\"\\\"Fusion\\\"\"
} else {
    DEFINES += SAK_STYLE_DEFAULT=\"\\\"macOS\\\"\"
}

#↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
# Application version
DEFINES += SAK_VERSION=\"\\\"3.4.1\\\"\"
# File information
win32 {
    VERSION = 3.4.1.0
}else {
    VERSION = 3.4.1
}
#↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
