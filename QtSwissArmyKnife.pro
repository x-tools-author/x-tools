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
    src/SAKGlobal.cc \
    src/console/SAKConsole.cc \
    src/console/SAKLogOutput.cc \
    src/main.cc \
    src/SAKApplication.cc \
    src/main_window/SAKMainWindow.cc \
    src/tabpage/SAKIODeviceWidget.cc \
    src/tabpage/SAKIODevice.cc \
    src/tabpage/SAKIODeviceControler.cc \
    src/tabpage/read_write_setting/SAKReadWriteSetting.cc \
    src/tabpage/auto_response/SAKAutoResponseItem.cc \
    src/tabpage/auto_response/SAKAutoResponseSettingPanel.cc \
    src/tabpage/highlighter/SAKHighlighter.cc \
    src/tabpage/highlighter/SAKHighlighterSettingPanel.cc \
    src/main_window/SAKVersion.cc \
    src/main_window/MoreInformation.cc

HEADERS += \
    src/SAKApplication.hh \
    src/SAKGlobal.hh \
    src/tabpage_serialport/SerialportSAKIODevice.hh \
    src/tabpage_serialport/SerialportSAKIODeviceControler.hh \
    src/tabpage_serialport/SerialportSAKIODeviceWidget.hh \
    src/tabpage_tcp_client/TcpSAKIODevice.hh \
    src/tabpage_tcp_client/TcpSAKIODeviceControler.hh \
    src/tabpage_tcp_client/TcpSAKIODeviceWidget.hh \
    src/tabpage_tcp_server/TcpServerSAKIODevice.hh \
    src/tabpage_tcp_server/TcpServerSAKIODeviceControler.hh \
    src/tabpage_tcp_server/TcpServerSAKIODeviceWidget.hh \
    src/tabpage_udp_client/UdpSAKIODevice.hh \
    src/tabpage_udp_client/UdpSAKIODeviceControler.hh \
    src/tabpage_udp_client/UdpSAKIODeviceWidget.hh \
    src/console/SAKConsole.hh \
    src/console/SAKLogOutput.hh \
    src/main_window/MoreInformation.hh \
    src/main_window/SAKMainWindow.hh \
    src/main_window/SAKVersion.hh \
    src/tabpage/SAKIODevice.hh \
    src/tabpage/SAKIODeviceControler.hh \
    src/tabpage/SAKIODeviceWidget.hh \
    src/tabpage/auto_response/SAKAutoResponseItem.hh \
    src/tabpage/auto_response/SAKAutoResponseSettingPanel.hh \
    src/tabpage/highlighter/SAKHighlighter.hh \
    src/tabpage/highlighter/SAKHighlighterSettingPanel.hh \
    src/tabpage/read_write_setting/SAKReadWriteSetting.hh

FORMS += \
    src/console/SAKConsole.ui \
    src/main_window/SAKMainWindow.ui \
    src/tabpage/SAKIODeviceWidget.ui \
    src/tabpage/auto_response/SAKAutoResponseItem.ui \
    src/tabpage/auto_response/SAKAutoResponseSettingPanel.ui \
    src/tabpage/highlighter/SAKHighlighterSettingPanel.ui \
    src/tabpage/read_write_setting/SAKReadWriteSetting.ui \
    src/main_window/SAKVersion.ui \
    src/main_window/MoreInformation.ui

INCLUDEPATH += \
    src \
    src/main_window \
    src/main_window/common \
    src/console \
    src/tabpage \
    src/tabpage/highlighter \
    src/tabpage/auto_response \
    src/tabpage/read_write_setting

RESOURCES += \
    SAKResources.qrc \
    stylesheet/qss.qrc

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
        src/tabpage_serialport/SerialportSAKIODeviceWidget.cc \
        src/tabpage_serialport/SerialportSAKIODevice.cc \
        src/tabpage_serialport/SerialportSAKIODeviceControler.cc
    HEADERS +=
    FORMS   += \
        src/tabpage_serialport/SerialportSAKIODeviceControler.ui
    INCLUDEPATH += \
        src/tabpage_serialport
}else {
    message( "该版本Qt可能不包含串口模块，已经忽略串口模块！（串口助手功能被屏蔽！）" )
}

# TCP 客户端
SOURCES += \
    src/tabpage_tcp_client/TcpSAKIODevice.cc \
    src/tabpage_tcp_client/TcpSAKIODeviceControler.cc \
    src/tabpage_tcp_client/TcpSAKIODeviceWidget.cc
HEADERS +=
FORMS   += \
    src/tabpage_tcp_client/TcpSAKIODeviceControler.ui
INCLUDEPATH += \
    src/tabpage_tcp_client

# TCP服务器
SOURCES += \
    src/tabpage_tcp_server/TcpServerSAKIODeviceWidget.cc \
    src/tabpage_tcp_server/TcpServerSAKIODevice.cc \
    src/tabpage_tcp_server/TcpServerSAKIODeviceControler.cc
HEADERS +=
FORMS   += \
    src/tabpage_tcp_server/TcpServerSAKIODeviceControler.ui
INCLUDEPATH += \
    src/tabpage_tcp_server

# UDP客户端
SOURCES += \
    src/tabpage_udp_client/UdpSAKIODevice.cc \
    src/tabpage_udp_client/UdpSAKIODeviceControler.cc \
    src/tabpage_udp_client/UdpSAKIODeviceWidget.cc
HEADERS +=
FORMS   += \
    src/tabpage_udp_client/UdpSAKIODeviceControler.ui
INCLUDEPATH += \
    src/tabpage_udp_client

DISTFILES += \
    other/update.json
