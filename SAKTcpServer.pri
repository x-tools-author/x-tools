#*******************************************************************************
# The file is encoding with utf-8 (with BOM)
#
# I write the comment with English, it's not because that I'm good at English,
# but for "installing B".
#
# Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
#******************************************************************************/
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
