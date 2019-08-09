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
HEADERS += \
    Src/Base/EDBaseApi.hh \
    Src/Base/SAKDialog.hh \
    Src/Base/SAKMessageBox.hh \
    Src/Base/SAKWidget.hh \
    Src/Common/SAKDebugPage/Other/AutoResponse/AutoResponseItemWidget.hh \
    Src/Common/SAKDebugPage/Other/AutoResponse/AutoResponseSettingWidget.hh \
    Src/Common/SAKDebugPage/Other/SAKOtherSettings.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/BaseTransmissionItemWidget.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/SerialPortTransmissionItemWidget.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TcpTransmissionItemWidget.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionPage.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionSettings.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/UdpTransmissionItemWidget.hh \
    Src/SAKGlobal.hh \
    Src/SAKVersion.hh \
    Src/SAKMainWindow.hh \
    Src/SAKApplication.hh \
    Src/MoreInformation.hh \
    Src/Console/SAKConsole.hh \
    Src/Console/SAKLogOutput.hh \
    Src/Common/SAKDebugPage/SAKDebugPage.hh \
    Src/Common/SAKDebugPage/SAKDataFactory.hh \
    Src/Common/SAKDebugPage/SAKCRCInterface.hh \
    Src/Common/SAKDebugPage/Other/Highlighter/SAKHighlighter.hh \
    Src/Common/SAKDebugPage/ReadWriteSetting/SAKReadWriteSetting.hh \
    Src/Common/SAKDebugPage/Other/Highlighter/SAKHighlighterSettingPanel.hh


SOURCES += \
    Src/Base/EDBaseApi.cc \
    Src/Base/SAKDialog.cc \
    Src/Base/SAKMessageBox.cc \
    Src/Base/SAKWidget.cc \
    Src/Common/SAKDebugPage/Other/AutoResponse/AutoResponseItemWidget.cc \
    Src/Common/SAKDebugPage/Other/AutoResponse/AutoResponseSettingWidget.cc \
    Src/Common/SAKDebugPage/Other/SAKOtherSettings.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/BaseTransmissionItemWidget.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/SerialPortTransmissionItemWidget.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TcpTransmissionItemWidget.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionPage.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionSettings.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/UdpTransmissionItemWidget.cc \
    Src/main.cc \
    Src/SAKGlobal.cc \
    Src/SAKVersion.cc \
    Src/SAKMainWindow.cc \
    Src/SAKApplication.cc \
    Src/MoreInformation.cc \
    Src/Console/SAKConsole.cc \
    Src/Console/SAKLogOutput.cc \
    Src/Common/SAKDebugPage/SAKDebugPage.cc \
    Src/Common/SAKDebugPage/SAKDataFactory.cc \
    Src/Common/SAKDebugPage/SAKCRCInterface.cc \
    Src/Common/SAKDebugPage/Other/Highlighter/SAKHighlighter.cc \
    Src/Common/SAKDebugPage/ReadWriteSetting/SAKReadWriteSetting.cc \
    Src/Common/SAKDebugPage/Other/Highlighter/SAKHighlighterSettingPanel.cc


FORMS += \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionPage.ui \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/SerialPortTransmissionItemWidget.ui \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TcpTransmissionItemWidget.ui \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionSettings.ui \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/UdpTransmissionItemWidget.ui \
    Src/SAKVersion.ui \
    Src/SAKMainWindow.ui \
    Src/MoreInformation.ui \
    Src/Console/SAKConsole.ui \
    Src/Common/SAKDebugPage/SAKDebugPage.ui \
    Src/Common/SAKDebugPage/Other/AutoResponse/AutoResponseItemWidget.ui \
    Src/Common/SAKDebugPage/ReadWriteSetting/SAKReadWriteSetting.ui \
    Src/Common/SAKDebugPage/Other/Highlighter/SAKHighlighterSettingPanel.ui \
    Src/Common/SAKDebugPage/Other/AutoResponse/AutoResponseSettingWidget.ui


INCLUDEPATH += \
    Src \
    Src/Base \
    Src/Console \
    Src/Common/SAKDebugPage \
    Src/Common/SAKDebugPage/Other/Highlighter \
    Src/Common/SAKDebugPage/Other/AutoResponse \
    Src/Common/SAKDebugPage/ReadWriteSetting \
    Src/Common/SAKDebugPage/Other/TransmissionSettings \
    Src/Common/SAKDebugPage/Other


RESOURCES += \
    SAKResources.qrc

#--------------------------------------------------------------------------------------------
#编译目录配置
UI_DIR      = $$OUT_PWD/ui
MOC_DIR     = $$OUT_PWD/moc
RCC_DIR     = $$OUT_PWD/res
OBJECTS_DIR = $$OUT_PWD/obj


#--------------------------------------------------------------------------------------------
# 子项目
include(SAKModulesManager.pri)

# 取消该宏的定义可以将串口模块屏蔽
winrt || linux-rasp-pi3-g++{
    DEFINES += SAK_NO_SERIALPORT_ASSISTANT
}

!contains(DEFINES, SAK_NO_SERIALPORT_ASSISTANT){
    QT  += serialport
    SOURCES += \
        Src/SerialportAssistant/SAKSerialportAssistant.cc \
        Src/SerialportAssistant/SAKSerialportAssistantController.cc \
        Src/SerialportAssistant/SAKTabPageSerialportAssistant.cc
    HEADERS += \
        Src/SerialportAssistant/SAKSerialportAssistant.hh \
        Src/SerialportAssistant/SAKSerialportAssistantController.hh \
        Src/SerialportAssistant/SAKTabPageSerialportAssistant.hh
    FORMS   += \
        Src/SerialportAssistant/SAKSerialportAssistantController.ui
    INCLUDEPATH += \
        Src/SerialportAssistant
}else {
    message( "该版本Qt可能不包含串口模块，已经忽略串口模块！（串口助手功能被屏蔽！）" )
}

win32{
    RC_ICONS = Windows.ico
}

# 解决msvc编译器中文乱码的问题
msvc:{
    QMAKE_CXXFLAGS += -execution-charset:utf-8
}
