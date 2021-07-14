DEFINES+=SAK_IMPORT_MODULE_VIRTUALDEVICE

contains(DEFINES, SAK_IMPORT_MODULE_VIRTUALDEVICE){
    FORMS += \
        $$PWD/SAKTestDeviceController.ui

    HEADERS += \
        $$PWD/SAKTestDebugPage.hh \
        $$PWD/SAKTestDevice.hh \
        $$PWD/SAKTestDeviceController.hh

    SOURCES += \
        $$PWD/SAKTestDebugPage.cc \
        $$PWD/SAKTestDevice.cc \
        $$PWD/SAKTestDeviceController.cc

    INCLUDEPATH += \
        $$PWD
}else {
    message("The Qt edition has no serialport module, the program will not has serialport debugging function.")
}


