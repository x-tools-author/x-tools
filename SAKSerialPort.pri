win32 {
    !winrt{
        QT  += serialport
        DEFINES+=SAK_IMPORT_COM_MODULE
    }
}

unix {
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
    message( "不支持串口调试功能，忽略串口调试模块！（串口调试功能已被屏蔽！）" )
}
