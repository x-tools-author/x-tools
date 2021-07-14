qtHaveModule(serialport){
    QT  += serialport
    DEFINES+=SAK_IMPORT_MODULE_SERIALPORT
}

contains(DEFINES, SAK_IMPORT_MODULE_SERIALPORT){
    FORMS += \
        $$PWD/src/debuggers/serialport/SAKSerialPortDeviceController.ui

    HEADERS += \
        $$PWD/src/debuggers/serialport/SAKSerialPortDebugPage.hh \
        $$PWD/src/debuggers/serialport/SAKSerialPortDevice.hh \
        $$PWD/src/debuggers/serialport/SAKSerialPortDeviceController.hh

    SOURCES += \
        $$PWD/src/debuggers/serialport/SAKSerialPortDebugPage.cc \
        $$PWD/src/debuggers/serialport/SAKSerialPortDevice.cc \
        $$PWD/src/debuggers/serialport/SAKSerialPortDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/debuggers/serialport
}else {
    message("The Qt edition has no serialport module, the program will not has serialport debugging function.")
}
