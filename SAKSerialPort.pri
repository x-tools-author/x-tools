qtHaveModule(serialport){
    QT  += serialport
    DEFINES+=SAK_IMPORT_COM_MODULE
}

contains(DEFINES, SAK_IMPORT_COM_MODULE){    
    SOURCES += \
        src/serialport/SAKSerialPortDebugPage.cc \
        src/serialport/SAKSerialPortDeviceController.cc \
        src/serialport/SAKSerialPortDevice.cc
    HEADERS += \
        src/serialport/SAKSerialPortDebugPage.hh \
        src/serialport/SAKSerialPortDeviceController.hh \
        src/serialport/SAKSerialPortDevice.hh
    FORMS   += \
        src/serialport/SAKSerialPortDeviceController.ui
    INCLUDEPATH += \
        src/serialport
}else {
    warning("The Qt edition has no serialport module, the program will not has serialport debuging functioin.")
}
