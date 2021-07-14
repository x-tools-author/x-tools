qtHaveModule(serialport){
    QT  += serialport
    DEFINES+=SAK_IMPORT_MODULE_SERIALPORT
}

contains(DEFINES, SAK_IMPORT_MODULE_SERIALPORT){
    FORMS += \
        $$PWD/SAKSerialPortDeviceController.ui

    HEADERS += \
        $$PWD/SAKSerialPortDebugPage.hh \
        $$PWD/SAKSerialPortDevice.hh \
        $$PWD/SAKSerialPortDeviceController.hh

    SOURCES += \
        $$PWD/SAKSerialPortDebugPage.cc \
        $$PWD/SAKSerialPortDevice.cc \
        $$PWD/SAKSerialPortDeviceController.cc

    INCLUDEPATH += \
        $$PWD
}else {
    message("The Qt edition has no serialport module, the program will not has serialport debugging function.")
}
