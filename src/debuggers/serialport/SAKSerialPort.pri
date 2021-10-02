qtHaveModule(serialport){
    QT  += serialport
    DEFINES+=SAK_IMPORT_MODULE_SERIALPORT
}

contains(DEFINES, SAK_IMPORT_MODULE_SERIALPORT){
    FORMS += \
        $$PWD/SAKSerialPortController.ui

    HEADERS += \
        $$PWD/SAKSerialPortController.hh \
        $$PWD/SAKSerialPortDebugger.hh \
        $$PWD/SAKSerialPortDevice.hh

    SOURCES += \
        $$PWD/SAKSerialPortController.cc \
        $$PWD/SAKSerialPortDebugger.cc \
        $$PWD/SAKSerialPortDevice.cc

    INCLUDEPATH += \
        $$PWD
}else {
    message("The Qt edition has no serialport module, the program will not has serialport debugging function.")
}
