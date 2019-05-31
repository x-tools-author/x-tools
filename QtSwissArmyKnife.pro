#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

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
    Src/SAKGlobal.cc \
    Src/Console/SAKConsole.cc \
    Src/Console/SAKLogOutput.cc \
    Src/TabpPage/SAKTabPage.cc \
    Src/main.cc \
    Src/SAKApplication.cc \
    Src/SAKMainWindow.cc \
    Src/TabpPage/SAKIODeviceWidget.cc \
    Src/TabpPage/SAKIODevice.cc \
    Src/TabpPage/SAKIODeviceControler.cc \
    Src/TabpPage/ReadWriteSetting/SAKReadWriteSetting.cc \
    Src/TabpPage/AutoResponse/SAKAutoResponseItem.cc \
    Src/TabpPage/AutoResponse/SAKAutoResponseSettingPanel.cc \
    Src/TabpPage/Highlighter/SAKHighlighter.cc \
    Src/TabpPage/Highlighter/SAKHighlighterSettingPanel.cc \
    Src/SAKVersion.cc \
    Src/MoreInformation.cc

HEADERS += \
    Src/SAKApplication.hh \
    Src/SAKGlobal.hh \
    Src/TabPageSerialportAssistant/SerialportSAKIODevice.hh \
    Src/TabPageSerialportAssistant/SerialportSAKIODeviceControler.hh \
    Src/TabPageSerialportAssistant/SerialportSAKIODeviceWidget.hh \
    Src/TabPageTCPClient/TcpSAKIODevice.hh \
    Src/TabPageTCPClient/TcpSAKIODeviceControler.hh \
    Src/TabPageTCPClient/TcpSAKIODeviceWidget.hh \
    Src/TabPageTCPServer/TcpServerSAKIODevice.hh \
    Src/TabPageTCPServer/TcpServerSAKIODeviceControler.hh \
    Src/TabPageTCPServer/TcpServerSAKIODeviceWidget.hh \
    Src/TabpPage/SAKTabPage.hh \
    Src/TabpageUDPClient/UdpSAKIODevice.hh \
    Src/TabpageUDPClient/UdpSAKIODeviceControler.hh \
    Src/TabpageUDPClient/UdpSAKIODeviceWidget.hh \
    Src/Console/SAKConsole.hh \
    Src/Console/SAKLogOutput.hh \
    Src/MoreInformation.hh \
    Src/SAKMainWindow.hh \
    Src/SAKVersion.hh \
    Src/TabpPage/SAKIODevice.hh \
    Src/TabpPage/SAKIODeviceControler.hh \
    Src/TabpPage/SAKIODeviceWidget.hh \
    Src/TabpPage/AutoResponse/SAKAutoResponseItem.hh \
    Src/TabpPage/AutoResponse/SAKAutoResponseSettingPanel.hh \
    Src/TabpPage/Highlighter/SAKHighlighter.hh \
    Src/TabpPage/Highlighter/SAKHighlighterSettingPanel.hh \
    Src/TabpPage/ReadWriteSetting/SAKReadWriteSetting.hh

FORMS += \
    Src/Console/SAKConsole.ui \
    Src/SAKMainWindow.ui \
    Src/TabpPage/SAKIODeviceWidget.ui \
    Src/TabpPage/AutoResponse/SAKAutoResponseItem.ui \
    Src/TabpPage/AutoResponse/SAKAutoResponseSettingPanel.ui \
    Src/TabpPage/Highlighter/SAKHighlighterSettingPanel.ui \
    Src/TabpPage/ReadWriteSetting/SAKReadWriteSetting.ui \
    Src/SAKVersion.ui \
    Src/MoreInformation.ui \
    Src/TabpPage/SAKTabPage.ui

INCLUDEPATH += \
    Src \
    Src/common \
    Src/Console \
    Src/TabpPage \
    Src/TabpPage/Highlighter \
    Src/TabpPage/AutoResponse \
    Src/TabpPage/ReadWriteSetting

RESOURCES += \
    SAKResources.qrc

win32{
    RC_ICONS = window.ico
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
RCC_DIR     = $$OUT_PWD/resources
UI_DIR      = $$OUT_PWD/ui

#--------------------------------------------------------------------------------------------
# 子项目
include(Src/Modules/SAKTools.pri)

# 取消该宏的定义可以将串口模块屏蔽
winrt || arm-linux{
    DEFINES += SAK_NO_SERIALPORT_ASSISTANT
}

!contains(DEFINES, SAK_NO_SERIALPORT_ASSISTANT){
    QT  += serialport
    SOURCES += \
        Src/TabPageSerialportAssistant/SerialportSAKIODeviceWidget.cc \
        Src/TabPageSerialportAssistant/SerialportSAKIODevice.cc \
        Src/TabPageSerialportAssistant/SerialportSAKIODeviceControler.cc
    HEADERS +=
    FORMS   += \
        Src/TabPageSerialportAssistant/SerialportSAKIODeviceControler.ui
    INCLUDEPATH += \
        Src/TabPageSerialportAssistant
}else {
    message( "该版本Qt可能不包含串口模块，已经忽略串口模块！（串口助手功能被屏蔽！）" )
}

# TCP 客户端
SOURCES += \
    Src/TabPageTCPClient/TcpSAKIODevice.cc \
    Src/TabPageTCPClient/TcpSAKIODeviceControler.cc \
    Src/TabPageTCPClient/TcpSAKIODeviceWidget.cc
HEADERS +=
FORMS   += \
    Src/TabPageTCPClient/TcpSAKIODeviceControler.ui
INCLUDEPATH += \
    Src/TabPageTCPClient

# TCP服务器
SOURCES += \
    Src/TabPageTCPServer/TcpServerSAKIODeviceWidget.cc \
    Src/TabPageTCPServer/TcpServerSAKIODevice.cc \
    Src/TabPageTCPServer/TcpServerSAKIODeviceControler.cc
HEADERS +=
FORMS   += \
    Src/TabPageTCPServer/TcpServerSAKIODeviceControler.ui
INCLUDEPATH += \
    Src/TabPageTCPServer

# UDP客户端
SOURCES += \
    Src/TabpageUDPClient/UdpSAKIODevice.cc \
    Src/TabpageUDPClient/UdpSAKIODeviceControler.cc \
    Src/TabpageUDPClient/UdpSAKIODeviceWidget.cc
HEADERS +=
FORMS   += \
    Src/TabpageUDPClient/UdpSAKIODeviceControler.ui
INCLUDEPATH += \
    Src/TabpageUDPClient

include(Modules/QtStyleSheet/QtStyleSheetApi.pri)

# 解决msvc编译器中文乱码的问题
msvc:{
    QMAKE_CXXFLAGS += -execution-charset:utf-8
}
