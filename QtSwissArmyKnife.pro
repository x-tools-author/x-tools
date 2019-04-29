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
    Src/SAKGlobal.cc \
    Src/console/SAKConsole.cc \
    Src/console/SAKLogOutput.cc \
    Src/main.cc \
    Src/SAKApplication.cc \
    Src/main_window/SAKMainWindow.cc \
    Src/tabpage/SAKIODeviceWidget.cc \
    Src/tabpage/SAKIODevice.cc \
    Src/tabpage/SAKIODeviceControler.cc \
    Src/tabpage/read_write_setting/SAKReadWriteSetting.cc \
    Src/tabpage/auto_response/SAKAutoResponseItem.cc \
    Src/tabpage/auto_response/SAKAutoResponseSettingPanel.cc \
    Src/tabpage/highlighter/SAKHighlighter.cc \
    Src/tabpage/highlighter/SAKHighlighterSettingPanel.cc \
    Src/main_window/SAKVersion.cc \
    Src/main_window/MoreInformation.cc

HEADERS += \
    Src/SAKApplication.hh \
    Src/SAKGlobal.hh \
    Src/tabpage_serialport/SerialportSAKIODevice.hh \
    Src/tabpage_serialport/SerialportSAKIODeviceControler.hh \
    Src/tabpage_serialport/SerialportSAKIODeviceWidget.hh \
    Src/tabpage_tcp_client/TcpSAKIODevice.hh \
    Src/tabpage_tcp_client/TcpSAKIODeviceControler.hh \
    Src/tabpage_tcp_client/TcpSAKIODeviceWidget.hh \
    Src/tabpage_tcp_server/TcpServerSAKIODevice.hh \
    Src/tabpage_tcp_server/TcpServerSAKIODeviceControler.hh \
    Src/tabpage_tcp_server/TcpServerSAKIODeviceWidget.hh \
    Src/tabpage_udp_client/UdpSAKIODevice.hh \
    Src/tabpage_udp_client/UdpSAKIODeviceControler.hh \
    Src/tabpage_udp_client/UdpSAKIODeviceWidget.hh \
    Src/console/SAKConsole.hh \
    Src/console/SAKLogOutput.hh \
    Src/main_window/MoreInformation.hh \
    Src/main_window/SAKMainWindow.hh \
    Src/main_window/SAKVersion.hh \
    Src/tabpage/SAKIODevice.hh \
    Src/tabpage/SAKIODeviceControler.hh \
    Src/tabpage/SAKIODeviceWidget.hh \
    Src/tabpage/auto_response/SAKAutoResponseItem.hh \
    Src/tabpage/auto_response/SAKAutoResponseSettingPanel.hh \
    Src/tabpage/highlighter/SAKHighlighter.hh \
    Src/tabpage/highlighter/SAKHighlighterSettingPanel.hh \
    Src/tabpage/read_write_setting/SAKReadWriteSetting.hh

FORMS += \
    Src/console/SAKConsole.ui \
    Src/main_window/SAKMainWindow.ui \
    Src/tabpage/SAKIODeviceWidget.ui \
    Src/tabpage/auto_response/SAKAutoResponseItem.ui \
    Src/tabpage/auto_response/SAKAutoResponseSettingPanel.ui \
    Src/tabpage/highlighter/SAKHighlighterSettingPanel.ui \
    Src/tabpage/read_write_setting/SAKReadWriteSetting.ui \
    Src/main_window/SAKVersion.ui \
    Src/main_window/MoreInformation.ui

INCLUDEPATH += \
    Src \
    Src/main_window \
    Src/main_window/common \
    Src/console \
    Src/tabpage \
    Src/tabpage/highlighter \
    Src/tabpage/auto_response \
    Src/tabpage/read_write_setting

RESOURCES += \
    SAKResources.qrc \
    StyleSheet/qss.qrc

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
RCC_DIR     = $$OUT_PWD/Resources
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
        Src/tabpage_serialport/SerialportSAKIODeviceWidget.cc \
        Src/tabpage_serialport/SerialportSAKIODevice.cc \
        Src/tabpage_serialport/SerialportSAKIODeviceControler.cc
    HEADERS +=
    FORMS   += \
        Src/tabpage_serialport/SerialportSAKIODeviceControler.ui
    INCLUDEPATH += \
        Src/tabpage_serialport
}else {
    message( "该版本Qt可能不包含串口模块，已经忽略串口模块！（串口助手功能被屏蔽！）" )
}

# TCP 客户端
SOURCES += \
    Src/tabpage_tcp_client/TcpSAKIODevice.cc \
    Src/tabpage_tcp_client/TcpSAKIODeviceControler.cc \
    Src/tabpage_tcp_client/TcpSAKIODeviceWidget.cc
HEADERS +=
FORMS   += \
    Src/tabpage_tcp_client/TcpSAKIODeviceControler.ui
INCLUDEPATH += \
    Src/tabpage_tcp_client

# TCP服务器
SOURCES += \
    Src/tabpage_tcp_server/TcpServerSAKIODeviceWidget.cc \
    Src/tabpage_tcp_server/TcpServerSAKIODevice.cc \
    Src/tabpage_tcp_server/TcpServerSAKIODeviceControler.cc
HEADERS +=
FORMS   += \
    Src/tabpage_tcp_server/TcpServerSAKIODeviceControler.ui
INCLUDEPATH += \
    Src/tabpage_tcp_server

# UDP客户端
SOURCES += \
    Src/tabpage_udp_client/UdpSAKIODevice.cc \
    Src/tabpage_udp_client/UdpSAKIODeviceControler.cc \
    Src/tabpage_udp_client/UdpSAKIODeviceWidget.cc
HEADERS +=
FORMS   += \
    Src/tabpage_udp_client/UdpSAKIODeviceControler.ui
INCLUDEPATH += \
    Src/tabpage_udp_client

DISTFILES += \
    Other/update.json
