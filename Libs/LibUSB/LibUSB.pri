LIB_USB_VERSION=1.0.23

contains(QT_ARCH, x86_64){
    LIB_USB_ARCH=x64
}else{
    LIB_USB_ARCH=Win32
}

CONFIG(debug, debug|release){
    LIB_USB_EDITION=Debug
}else{
    LIB_USB_EDITION=Release
}

INCLUDEPATH += \
    $${PWD}/$${LIB_USB_VERSION}

win32 {
    LIBS    += -L$${PWD}/$${LIB_USB_VERSION}/$${LIB_USB_ARCH}/$${LIB_USB_EDITION}/lib -llibusb-1.0
    LIBS    += -lAdvapi32
}else{
    error("usb模块不支持该平台")
}

