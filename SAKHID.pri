win32 {
    DEFINES+=SAK_IMPORT_HID_MODULE
}

unix {
    DEFINES+=SAK_IMPORT_HID_MODULE
}

contains(DEFINES, SAK_IMPORT_HID_MODULE){
include(3rdParty/HidAPI/HidApi.pri)
}else {
    message( "不支持HID调试功能，忽略HID调试模块！（HID调试功能已被屏蔽！）" )
}


contains(DEFINES, SAK_IMPORT_HID_MODULE){
HEADERS += \
    Src/DebugPage/HidDebug/SAKHidDebugPage.hh \
    Src/DebugPage/HidDebug/SAKHidDevice.hh \
    Src/DebugPage/HidDebug/SAKHidDeviceController.hh

SOURCES += \
    Src/DebugPage/HidDebug/SAKHidDebugPage.cc \
    Src/DebugPage/HidDebug/SAKHidDevice.cc \
    Src/DebugPage/HidDebug/SAKHidDeviceController.cc

FORMS   += \
    Src/DebugPage/HidDebug/SAKHidDeviceController.ui

INCLUDEPATH += \
    Src/DebugPage/HidDebug
}
