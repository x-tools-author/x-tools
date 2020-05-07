qtHaveModule(serialport){
    QT  += serialport
    DEFINES+=SAK_IMPORT_COM_MODULE
}

contains(DEFINES, SAK_IMPORT_COM_MODULE){
    HEADERS += \
        $$PWD/src/debugger/device/SAKDebuggerDeviceSerialport.hh

    SOURCES += \
        $$PWD/src/debugger/device/SAKDebuggerDeviceSerialport.cc
}else{
    message("The Qt edition has no serialport module, the program will not has serialport debuging functioin.")
}

