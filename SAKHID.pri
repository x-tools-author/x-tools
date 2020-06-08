win32 {
    # hidapi 优先使用源码
    exists(3rd/hidapi/windows/hid.c){
        LIBS += -lsetupapi
        HEADERS += 3rd/hidapi/hidapi/hidapi.h
        SOURCES += 3rd/hidapi/windows/hid.c
        INCLUDEPATH += 3rd/hidapi/hidapi
        # 屏蔽64位源码编译时的警告
        QMAKE_CFLAGS_WARN_OFF += /wd4267

        DEFINES += SAK_IMPORT_HID_MODULE
    }else{
        include(libs/hidapi/SAKHidApiManager.pri)
    }
}

contains(DEFINES, SAK_IMPORT_HID_MODULE){
    FORMS += \
        $$PWD/src/pages/hid/SAKHidDeviceController.ui

    HEADERS += \
        $$PWD/src/pages/hid/SAKHidDebugPage.hh \
        $$PWD/src/pages/hid/SAKHidDevice.hh \
        $$PWD/src/pages/hid/SAKHidDeviceController.hh

    SOURCES += \
        $$PWD/src/pages/hid/SAKHidDebugPage.cc \
        $$PWD/src/pages/hid/SAKHidDevice.cc \
        $$PWD/src/pages/hid/SAKHidDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/pages/hid
}else {
    message("The HID module is not supported by the platform, the program will not has hid debuging function.")
}
