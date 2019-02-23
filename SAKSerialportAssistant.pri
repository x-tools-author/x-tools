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
    src/SerialPort/SerialportSAKIODeviceWidget.cpp \
    src/SerialPort/SerialportSAKIODevice.cpp \
    src/SerialPort/SerialportSAKIODeviceControler.cpp
HEADERS += \
    src/SerialPort/SerialportSAKIODeviceWidget.h \
    src/SerialPort/SerialportSAKIODevice.h \
    src/SerialPort/SerialportSAKIODeviceControler.h
FORMS   += \
    src/SerialPort/SerialportSAKIODeviceControler.ui
INCLUDEPATH += \
    src/SerialPort
