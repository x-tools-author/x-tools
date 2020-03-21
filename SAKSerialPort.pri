qtHaveModule(serialport){
    QT  += serialport
    DEFINES+=SAK_IMPORT_COM_MODULE
}

contains(DEFINES, SAK_IMPORT_COM_MODULE){    
    FORMS += \
        $$PWD/src/serialport/SAKSerialPortDeviceController.ui

    HEADERS += \
        $$PWD/src/serialport/SAKSerialPortDebugPage.hh \
        $$PWD/src/serialport/SAKSerialPortDevice.hh \
        $$PWD/src/serialport/SAKSerialPortDeviceController.hh

    SOURCES += \
        $$PWD/src/serialport/SAKSerialPortDebugPage.cc \
        $$PWD/src/serialport/SAKSerialPortDevice.cc \
        $$PWD/src/serialport/SAKSerialPortDeviceController.cc

    INCLUDEPATH += \
        $$PWD/src/serialport
}else {
    warning("The Qt edition has no serialport module, the program will not has serialport debuging functioin.")
}
