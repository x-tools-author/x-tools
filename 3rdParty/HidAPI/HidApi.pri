HIDVERSION=0.9.0
LIBS    += -lsetupapi

INCLUDEPATH += \
    $${PWD}/$${HIDVERSION}

HEADERS += $${PWD}/$${HIDVERSION}/HidApi.h

win32 {
    SOURCES += $${PWD}/$${HIDVERSION}/HidWindows.c
}else{
    unix {
        SOURCES += $${PWD}/$${HIDVERSION}/HidLinux.c
    }else {
        macOS {
            SOURCES += $${PWD}/$${HIDVERSION}/HidMac.c
        }else{
            error("HID调试模块不支持该平台")
        }
    }
}




