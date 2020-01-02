qtHaveModule(serialport){
    QT  += serialport
    DEFINES+=SAK_IMPORT_COM_MODULE
}

contains(DEFINES, SAK_IMPORT_COM_MODULE){    
    SOURCES += \
        ui/widgets/serialport/SAKSerialPortDebugPage.cc \
        ui/widgets/serialport/SAKSerialPortDeviceController.cc \
        ui/widgets/serialport/SAKSerialPortDevice.cc
    HEADERS += \
        ui/widgets/serialport/SAKSerialPortDebugPage.hh \
        ui/widgets/serialport/SAKSerialPortDeviceController.hh \
        ui/widgets/serialport/SAKSerialPortDevice.hh
    FORMS   += \
        ui/widgets/serialport/SAKSerialPortDeviceController.ui
    INCLUDEPATH += \
        ui/widgets/serialport
}else {
    warning("The Qt edition has no serialport module, the program will not has serialport debuging functioin.")
}
