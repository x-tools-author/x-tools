win32 {
    QT  += serialport
    DEFINES+=SAK_IMPORT_COM_MODULE
}

contains(DEFINES, SAK_IMPORT_COM_MODULE){    
    SOURCES += \
        Src/DebugPage/ComDebug/SAKSerialPortDebugPage.cc \
        Src/DebugPage/ComDebug/SAKSerialPortDeviceController.cc \
        Src/DebugPage/ComDebug/SAKSerialPortDevice.cc
    HEADERS += \
        Src/DebugPage/ComDebug/SAKSerialPortDebugPage.hh \
        Src/DebugPage/ComDebug/SAKSerialPortDeviceController.hh \
        Src/DebugPage/ComDebug/SAKSerialPortDevice.hh
    FORMS   += \
        Src/DebugPage/ComDebug/SAKSerialPortDeviceController.ui
    INCLUDEPATH += \
        Src/DebugPage/ComDebug
}else {
    message( "不支持Qt串口功能，忽略串口模块！（串口调试功能已被屏蔽！）" )
}
