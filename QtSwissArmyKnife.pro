#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport network

TARGET = QtSwissArmyKnife
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
        src/ui/mainwindow.cpp \
    src/SerialPort/SerialportSAKIODeviceWidget.cpp \
    src/SerialPort/SerialportSAKIODevice.cpp \
    src/SAKApplication.cpp \
    src/console/console.cpp \
    src/comment/SAKIODeviceWidget.cpp \
    src/SerialPort/SerialportSAKIODeviceControler.cpp \
    src/comment/SAKIODevice.cpp \
    src/comment/SAKIODeviceControler.cpp \
    src/UdpClient/UdpSAKIODevice.cpp \
    src/UdpClient/UdpSAKIODeviceControler.cpp \
    src/UdpClient/UdpSAKIODeviceWidget.cpp \
    src/TcpClient/TcpSAKIODevice.cpp \
    src/TcpClient/TcpSAKIODeviceControler.cpp \
    src/TcpClient/TcpSAKIODeviceWidget.cpp \
    src/TcpServer/TcpServerSAKIODevice.cpp \
    src/TcpServer/TcpServerSAKIODeviceControler.cpp \
    src/TcpServer/TcpServerSAKIODeviceWidget.cpp \
    src/comment/SAKSettings.cpp

HEADERS += \
        src/ui/mainwindow.h \
    src/SerialPort/SerialportSAKIODeviceWidget.h \
    src/SerialPort/SerialportSAKIODevice.h \
    src/SAKApplication.h \
    src/console/console.h \
    src/comment/SAKIODeviceWidget.h \
    src/SerialPort/SerialportSAKIODeviceControler.h \
    src/comment/SAKIODevice.h \
    src/comment/SAKIODeviceControler.h \
    src/UdpClient/UdpSAKIODevice.h \
    src/UdpClient/UdpSAKIODeviceControler.h \
    src/UdpClient/UdpSAKIODeviceWidget.h \
    src/TcpClient/TcpSAKIODevice.h \
    src/TcpClient/TcpSAKIODeviceControler.h \
    src/TcpClient/TcpSAKIODeviceWidget.h \
    src/TcpServer/TcpServerSAKIODevice.h \
    src/TcpServer/TcpServerSAKIODeviceControler.h \
    src/TcpServer/TcpServerSAKIODeviceWidget.h \
    src/comment/SAKSettings.h

FORMS += \
        src/ui/mainwindow.ui \
    src/console/console.ui \
    src/comment/SAKIODeviceWidget.ui \
    src/SerialPort/SerialportSAKIODeviceControler.ui \
    src/UdpClient/UdpSAKIODeviceControler.ui \
    src/TcpClient/TcpSAKIODeviceControler.ui \
    src/TcpServer/TcpServerSAKIODeviceControler.ui

INCLUDEPATH += \
    src/ui \
    src/ui/common \
    src/SerialPort \
    src/UdpClient \
    src/TcpClient \
    src/TcpServer \
    src/console \
    src/comment

#--------------------------------------------------------------------------------------------
# 子项目
include(QtSwissArmyKnifeConfigure.pri)
include(QtSwissArmyKnifeSetup.pri)

