qtHaveModule(serialport){
    QT  += serialport
    DEFINES+=SAK_IMPORT_MODULE_SERIALPORT
}

contains(DEFINES, SAK_IMPORT_MODULE_SERIALPORT){
    FORMS += \
        $$PWD/src/pages/serialport/SAKSerialPortDeviceController.ui

    HEADERS += \
        $$PWD/src/pages/serialport/SAKSerialPortDebugPage.hh \
        $$PWD/src/pages/serialport/SAKSerialPortDevice.hh \
        $$PWD/src/pages/serialport/SAKSerialPortDeviceController.hh

    SOURCES += \
        $$PWD/src/pages/serialport/SAKSerialPortDebugPage.cc \
        $$PWD/src/pages/serialport/SAKSerialPortDevice.cc \
        $$PWD/src/pages/serialport/SAKSerialPortDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/pages/serialport
}else {
    message("The Qt edition has no serialport module, the program will not has serialport debugging function.")
}
