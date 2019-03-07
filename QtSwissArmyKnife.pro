#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

# 软件版本号
#DEFINES += SAK_VERSION=\"\\\"1.0.0\\\"\"

TARGET = QtSwissArmyKnife

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, unTabPage the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/MainWindow/mainwindow.cpp \
    src/SAKApplication.cpp \
    src/TabPage/SAKIODeviceWidget.cpp \
    src/TabPage/SAKIODevice.cpp \
    src/TabPage/SAKIODeviceControler.cpp \
    src/TabPage/AutoResponseItem.cpp \
    src/TabPage/AutoResponseSettingPanel.cpp \
    src/TabPage/SAKHighlighter.cpp \
    src/TabPage/SAKHighlighterSettingPanel.cpp \
    src/TabPage/SAKHighlighterLabel.cpp \
    src/MainWindow/SAKVersion.cpp

HEADERS += \
    src/MainWindow/mainwindow.h \
    src/SAKApplication.h \
    src/TabPage/SAKIODeviceWidget.h \
    src/TabPage/SAKIODevice.h \
    src/TabPage/SAKIODeviceControler.h \
    src/TabPage/AutoResponseItem.h \
    src/TabPage/AutoResponseSettingPanel.h \
    src/TabPage/SAKHighlighter.h \
    src/TabPage/SAKHighlighterSettingPanel.h \
    src/TabPage/SAKHighlighterLabel.h \
    src/MainWindow/SAKVersion.h

FORMS += \
    src/MainWindow/mainwindow.ui \
    src/TabPage/SAKIODeviceWidget.ui \
    src/TabPage/AutoResponseItem.ui \
    src/TabPage/AutoResponseSettingPanel.ui \
    src/TabPage/SAKHighlighterSettingPanel.ui \
    src/TabPage/SAKHighlighterLabel.ui \
    src/MainWindow/SAKVersion.ui

INCLUDEPATH += \
    src \
    src/MainWindow \
    src/MainWindow/common \
    src/UdpClient \
    src/TcpClient \
    src/TcpServer \
    src/console \
    src/TabPage

RESOURCES += \
    resource/images.qrc

win32{
    RC_ICONS = resource/icon/window.ico
}

# 静态编译版本不需要部署发布（静态编译时，禁用下面的）
DEFINES += NOT_USING_STATIC_EDITION

# 编译后不发布部署(发布时采用静态版本进行Qt进行编译)
#win32{
#    contains(DEFINES, NOT_USING_STATIC_EDITION){
#        CONFIG(debug, debug|release) {
#            win32-msvc {
#                QMAKE_POST_LINK += $$escape_expand(\\n) $$[QT_INSTALL_BINS]/windeployqt.exe --no-compiler-runtime --no-translations $$OUT_PWD/debug/$${TARGET}.exe $$escape_expand(\\n)
#            }
#        } else {
#            win32-msvc {
#                QMAKE_POST_LINK += $$escape_expand(\\n) $$[QT_INSTALL_BINS]/windeployqt.exe --no-compiler-runtime --no-translations $$OUT_PWD/release/$${TARGET}.exe $$escape_expand(\\n)
#            }
#        }
#    }
#}

#--------------------------------------------------------------------------------------------
#编译目录配置
MOC_DIR     = $$OUT_PWD/moc
OBJECTS_DIR = $$OUT_PWD/obj
RCC_DIR     = $$OUT_PWD/resource
UI_DIR      = $$OUT_PWD/ui

#--------------------------------------------------------------------------------------------
# 子项目
include(SAKTools.pri)

# 取消该宏的定义可以将串口模块屏蔽
winrt || arm-linux{
    DEFINES += SAK_NO_SERIALPORT_ASSISTANT
}

!contains(DEFINES, SAK_NO_SERIALPORT_ASSISTANT){
    QT  += serialport
    SOURCES += \
        src/TabPageSerialPort/SerialportSAKIODeviceWidget.cpp \
        src/TabPageSerialPort/SerialportSAKIODevice.cpp \
        src/TabPageSerialPort/SerialportSAKIODeviceControler.cpp
    HEADERS += \
        src/TabPageSerialPort/SerialportSAKIODeviceWidget.h \
        src/TabPageSerialPort/SerialportSAKIODevice.h \
        src/TabPageSerialPort/SerialportSAKIODeviceControler.h
    FORMS   += \
        src/TabPageSerialPort/SerialportSAKIODeviceControler.ui
    INCLUDEPATH += \
        src/TabPageSerialPort
}else {
    message( "该版本Qt可能不包含串口模块，已经忽略串口模块！（串口助手功能被屏蔽！）" )
}

# TCP 客户端
SOURCES += \
    src/TabPageTcpClient/TcpSAKIODevice.cpp \
    src/TabPageTcpClient/TcpSAKIODeviceControler.cpp \
    src/TabPageTcpClient/TcpSAKIODeviceWidget.cpp
HEADERS += \
    src/TabPageTcpClient/TcpSAKIODevice.h \
    src/TabPageTcpClient/TcpSAKIODeviceControler.h \
    src/TabPageTcpClient/TcpSAKIODeviceWidget.h
FORMS   += \
    src/TabPageTcpClient/TcpSAKIODeviceControler.ui
INCLUDEPATH += \
    src/TabPageTcpClient

# TCP服务器
SOURCES += \
    src/TabPageTcpServer/TcpServerSAKIODeviceWidget.cpp \
    src/TabPageTcpServer/TcpServerSAKIODevice.cpp \
    src/TabPageTcpServer/TcpServerSAKIODeviceControler.cpp
HEADERS += \
    src/TabPageTcpServer/TcpServerSAKIODeviceWidget.h \
    src/TabPageTcpServer/TcpServerSAKIODevice.h \
    src/TabPageTcpServer/TcpServerSAKIODeviceControler.h
FORMS   += \
    src/TabPageTcpServer/TcpServerSAKIODeviceControler.ui
INCLUDEPATH += \
    src/TabPageTcpServer

# UDP客户端
SOURCES += \
    src/TabPageUdpClient/UdpSAKIODevice.cpp \
    src/TabPageUdpClient/UdpSAKIODeviceControler.cpp \
    src/TabPageUdpClient/UdpSAKIODeviceWidget.cpp
HEADERS += \
    src/TabPageUdpClient/UdpSAKIODevice.h \
    src/TabPageUdpClient/UdpSAKIODeviceControler.h \
    src/TabPageUdpClient/UdpSAKIODeviceWidget.h
FORMS   += \
    src/TabPageUdpClient/UdpSAKIODeviceControler.ui
INCLUDEPATH += \
    src/TabPageUdpClient


DISTFILES += \
    other/update.json \
    other/developerLog.txt \
    other/bugManager.txt
