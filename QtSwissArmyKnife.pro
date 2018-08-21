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
    src/ui/common/outputwindow.cpp \
    src/SerialPort/SerialportSAKIODeviceWidget.cpp \
    src/ui/common/inputwindow.cpp \
    src/ui/common/senddatapanel.cpp \
    src/SerialPort/SerialportSAKIODevice.cpp \
    src/SAKApplication.cpp \
    src/UdpClient/UdpClientAssistant.cpp \
    src/UdpClient/UdpClientWidget.cpp \
    src/TcpClient/TCPClientAssistant.cpp \
    src/TcpClient/TCPClientWidget.cpp \
    src/TcpServer/TCPServerAssistant.cpp \
    src/TcpServer/TCPServerWidget.cpp \
    src/ui/console/console.cpp \
    src/comment/SAKIODeviceWidget.cpp \
    src/SerialPort/SerialportSAKIODeviceControler.cpp \
    src/comment/SAKIODevice.cpp \
    src/comment/SAKIODeviceControler.cpp

HEADERS += \
        src/ui/mainwindow.h \
    src/ui/common/outputwindow.h \
    src/SerialPort/SerialportSAKIODeviceWidget.h \
    src/ui/common/inputwindow.h \
    src/ui/common/senddatapanel.h \
    src/SerialPort/SerialportSAKIODevice.h \
    src/SAKApplication.h \
    src/UdpClient/UdpClientAssistant.h \
    src/UdpClient/UdpClientWidget.h \
    src/TcpClient/TCPClientAssistant.h \
    src/TcpClient/TCPClientWidget.h \
    src/TcpServer/TCPServerAssistant.h \
    src/TcpServer/TCPServerWidget.h \
    src/ui/console/console.h \
    src/comment/SAKIODeviceWidget.h \
    src/SerialPort/SerialportSAKIODeviceControler.h \
    src/comment/SAKIODevice.h \
    src/comment/SAKIODeviceControler.h

FORMS += \
        src/ui/mainwindow.ui \
    src/ui/common/outputwindow.ui \
    src/ui/common/inputwindow.ui \
    src/ui/common/senddatapanel.ui \
    src/UdpClient/UdpClientWidget.ui \
    src/TcpClient/TCPClientWidget.ui \
    src/TcpServer/TCPServerWidget.ui \
    src/ui/console/console.ui \
    src/comment/SAKIODeviceWidget.ui \
    src/SerialPort/SerialportSAKIODeviceControler.ui

INCLUDEPATH += \
    src/ui \
    src/ui/common \
    src/SerialPort \
    src/UdpClient \
    src/TcpClient \
    src/TcpServer \
    src/ui/console \
    src/comment

#--------------------------------------------------------------------------------------------
# 子项目
include(QtSwissArmyKnifeConfigure.pri)
include(QtSwissArmyKnifeSetup.pri)

