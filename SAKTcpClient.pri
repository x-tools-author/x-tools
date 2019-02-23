#*******************************************************************************
# The file is encoding with utf-8 (with BOM)
#
# I write the comment with English, it's not because that I'm good at English,
# but for "installing B".
#
# Copyright (C) 2018-2019 wuhai persionnal. All rights reserved.
#******************************************************************************/
!contains(QT, serialport){
greaterThan(QT_MAJOR_VERSION, 4): QT += serialport
}

SOURCES += \
    src/TcpClient/TcpSAKIODevice.cpp \
    src/TcpClient/TcpSAKIODeviceControler.cpp \
    src/TcpClient/TcpSAKIODeviceWidget.cpp
HEADERS += \
    src/TcpClient/TcpSAKIODevice.h \
    src/TcpClient/TcpSAKIODeviceControler.h \
    src/TcpClient/TcpSAKIODeviceWidget.h
FORMS   += \
    src/TcpClient/TcpSAKIODeviceControler.ui
INCLUDEPATH += \
    src/TcpClient
