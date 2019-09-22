win32 {
    DEFINES+=SAK_IMPORT_USB_MODULE
}

contains(DEFINES, SAK_IMPORT_USB_MODULE){
include(Libs/LibUSB/LibUSB.pri)
}else {
    message( "不支持USB调试功能，忽略USB调试模块！（USB调试功能已被屏蔽！）" )
}

contains(DEFINES, SAK_IMPORT_USB_MODULE){
HEADERS += \
    Src/DebugPage/UsbDebug/SAKUsbDebugPage.hh \
    Src/DebugPage/UsbDebug/SAKUsbDevice.hh \
    Src/DebugPage/UsbDebug/SAKUsbDeviceController.hh

SOURCES += \
    Src/DebugPage/UsbDebug/SAKUsbDebugPage.cc \
    Src/DebugPage/UsbDebug/SAKUsbDevice.cc \
    Src/DebugPage/UsbDebug/SAKUsbDeviceController.cc

FORMS   += \
    Src/DebugPage/UsbDebug/SAKUsbDeviceController.ui

INCLUDEPATH += \
    Src/DebugPage/USBDebug
}
