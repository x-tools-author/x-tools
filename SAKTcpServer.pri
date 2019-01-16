!contains(QT, serialport){
greaterThan(QT_MAJOR_VERSION, 4): QT += network
}

SOURCES += \
    src/TcpServer/TcpServerSAKIODeviceWidget.cpp \
    src/TcpServer/TcpServerSAKIODevice.cpp \
    src/TcpServer/TcpServerSAKIODeviceControler.cpp
HEADERS += \
    src/TcpServer/TcpServerSAKIODeviceWidget.h \
    src/TcpServer/TcpServerSAKIODevice.h \
    src/TcpServer/TcpServerSAKIODeviceControler.h
FORMS   += \
    src/TcpServer/TcpServerSAKIODeviceControler.ui
INCLUDEPATH += \
    src/TcpServer
