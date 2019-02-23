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
