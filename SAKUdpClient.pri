!contains(QT, serialport){
greaterThan(QT_MAJOR_VERSION, 4): QT += serialport
}

SOURCES += \
    src/UdpClient/UdpSAKIODevice.cpp \
    src/UdpClient/UdpSAKIODeviceControler.cpp \
    src/UdpClient/UdpSAKIODeviceWidget.cpp
HEADERS += \
    src/UdpClient/UdpSAKIODevice.h \
    src/UdpClient/UdpSAKIODeviceControler.h \
    src/UdpClient/UdpSAKIODeviceWidget.h
FORMS   += \
    src/UdpClient/UdpSAKIODeviceControler.ui
INCLUDEPATH += \
    src/SerialPort
