#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------

QT += core gui xml charts

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
    Src/Base/SAKBase.hh \
    Src/Base/SAKDialog.hh \
    Src/Base/SAKMessageBox.hh \
    Src/Base/SAKWidget.hh \
    Src/Common/SAKDebugPage/Chart/MoreChart/MoreChartWidget.hh \
    Src/Common/SAKDebugPage/Chart/SAKChartManager.hh \
    Src/Common/SAKDebugPage/Chart/Throughput/ThroughputWidget.hh \
    Src/Common/SAKDebugPage/Device/DebugPageDeviceManager.hh \
    Src/Common/SAKDebugPage/Input/DebugPageInputManager.hh \
    Src/Common/SAKDebugPage/Input/InputDataFactory.hh \
    Src/Common/SAKDebugPage/Input/InputDataItem.hh \
    Src/Common/SAKDebugPage/Message/DebugPageMessageManager.hh \
    Src/Common/SAKDebugPage/Other/AutoResponseSettings/AutoResponseItemWidget.hh \
    Src/Common/SAKDebugPage/Other/AutoResponseSettings/AutoResponseSettingWidget.hh \
    Src/Common/SAKDebugPage/Other/FormatSettings/FormatSettingsWidget.hh \
    Src/Common/SAKDebugPage/Other/HighlightSettings/HighlightSettings.hh \
    Src/Common/SAKDebugPage/Other/HighlightSettings/HighlightSettingsWidget.hh \
    Src/Common/SAKDebugPage/Other/MoreOtherSettings/MoreOtherSettingsWidget.hh \
    Src/Common/SAKDebugPage/Other/ReadWriteSettings/ReadWriteSettingsWidget.hh \
    Src/Common/SAKDebugPage/Other/SAKOtherSettings.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/BaseTransmissionItemWidget.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/SerialPortTransmissionItemWidget.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TcpTransmissionItemWidget.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionPage.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionSettings.hh \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/UdpTransmissionItemWidget.hh \
    Src/Common/SAKDebugPage/Output/DebugPageOutputManager.hh \
    Src/Common/SAKDebugPage/Output/OutputDataFactory.hh \
    Src/Common/SAKDebugPage/Output/Save/SaveOutputDataSettings.hh \
    Src/Common/SAKDebugPage/Output/Save/SaveOutputDataThread.hh \
    Src/Common/SAKDebugPage/Statistics/SAKStatisticsManager.hh \
    Src/Common/SAKToolDialog/SAKToolDialog.hh \
    Src/HIDDebug/SAKHidDebugPage.hh \
    Src/HIDDebug/SAKHidDevice.hh \
    Src/HIDDebug/SAKHidDeviceController.hh \
    Src/SAKGlobal.hh \
    Src/SAKVersion.hh \
    Src/SAKMainWindow.hh \
    Src/SAKApplication.hh \
    Src/MoreInformation.hh \
    Src/Console/SAKConsole.hh \
    Src/Console/SAKLogOutput.hh \
    Src/Common/SAKDebugPage/SAKDebugPage.hh \
    Src/Base/SAKCRCInterface.hh \
    Src/TcpClient/SAKTcpClientDebugPage.hh \
    Src/TcpClient/SAKTcpClientDevice.hh \
    Src/TcpClient/SAKTcpClientDeviceController.hh \
    Src/TcpServer/SAKTcpServerDebugPage.hh \
    Src/TcpServer/SAKTcpServerDevice.hh \
    Src/TcpServer/SAKTcpServerDeviceController.hh \
    Src/UdpDebug/SAKUdpDebugPage.hh \
    Src/UdpDebug/SAKUdpDevice.hh \
    Src/UdpDebug/SAKUdpDeviceController.hh


SOURCES += \
    Src/Base/SAKBase.cc \
    Src/Base/SAKDialog.cc \
    Src/Base/SAKMessageBox.cc \
    Src/Base/SAKWidget.cc \
    Src/Common/SAKDebugPage/Chart/MoreChart/MoreChartWidget.cc \
    Src/Common/SAKDebugPage/Chart/SAKChartManager.cc \
    Src/Common/SAKDebugPage/Chart/Throughput/ThroughputWidget.cc \
    Src/Common/SAKDebugPage/Device/DebugPageDeviceManager.cc \
    Src/Common/SAKDebugPage/Input/DebugPageInputManager.cc \
    Src/Common/SAKDebugPage/Input/InputDataFactory.cc \
    Src/Common/SAKDebugPage/Input/InputDataItem.cc \
    Src/Common/SAKDebugPage/Message/DebugPageMessageManager.cc \
    Src/Common/SAKDebugPage/Other/AutoResponseSettings/AutoResponseItemWidget.cc \
    Src/Common/SAKDebugPage/Other/AutoResponseSettings/AutoResponseSettingWidget.cc \
    Src/Common/SAKDebugPage/Other/FormatSettings/FormatSettingsWidget.cc \
    Src/Common/SAKDebugPage/Other/HighlightSettings/HighlightSettings.cc \
    Src/Common/SAKDebugPage/Other/HighlightSettings/HighlightSettingsWidget.cc \
    Src/Common/SAKDebugPage/Other/MoreOtherSettings/MoreOtherSettingsWidget.cc \
    Src/Common/SAKDebugPage/Other/ReadWriteSettings/ReadWriteSettingsWidget.cc \
    Src/Common/SAKDebugPage/Other/SAKOtherSettings.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/BaseTransmissionItemWidget.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/SerialPortTransmissionItemWidget.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TcpTransmissionItemWidget.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionPage.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionSettings.cc \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/UdpTransmissionItemWidget.cc \
    Src/Common/SAKDebugPage/Output/DebugPageOutputManager.cc \
    Src/Common/SAKDebugPage/Output/OutputDataFactory.cc \
    Src/Common/SAKDebugPage/Output/Save/SaveOutputDataSettings.cc \
    Src/Common/SAKDebugPage/Output/Save/SaveOutputDataThread.cc \
    Src/Common/SAKDebugPage/Statistics/SAKStatisticsManager.cc \
    Src/Common/SAKToolDialog/SAKToolDialog.cc \
    Src/HIDDebug/SAKHidDebugPage.cc \
    Src/HIDDebug/SAKHidDevice.cc \
    Src/HIDDebug/SAKHidDeviceController.cc \   
    Src/TcpClient/SAKTcpClientDebugPage.cc \
    Src/TcpClient/SAKTcpClientDevice.cc \
    Src/TcpClient/SAKTcpClientDeviceController.cc \
    Src/TcpServer/SAKTcpServerDebugPage.cc \
    Src/TcpServer/SAKTcpServerDevice.cc \
    Src/TcpServer/SAKTcpServerDeviceController.cc \
    Src/UdpDebug/SAKUdpDebugPage.cc \
    Src/UdpDebug/SAKUdpDevice.cc \
    Src/UdpDebug/SAKUdpDeviceController.cc \
    Src/main.cc \
    Src/SAKGlobal.cc \
    Src/SAKVersion.cc \
    Src/SAKMainWindow.cc \
    Src/SAKApplication.cc \
    Src/MoreInformation.cc \
    Src/Console/SAKConsole.cc \
    Src/Console/SAKLogOutput.cc \
    Src/Common/SAKDebugPage/SAKDebugPage.cc \
    Src/Base/SAKCRCInterface.cc


FORMS += \
    Src/Common/SAKDebugPage/Chart/MoreChart/MoreChartWidget.ui \
    Src/Common/SAKDebugPage/Input/InputDataItem.ui \
    Src/Common/SAKDebugPage/Other/FormatSettings/FormatSettingsWidget.ui \
    Src/Common/SAKDebugPage/Other/MoreOtherSettings/MoreOtherSettingsWidget.ui \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionPage.ui \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/SerialPortTransmissionItemWidget.ui \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TcpTransmissionItemWidget.ui \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/TransmissionSettings.ui \
    Src/Common/SAKDebugPage/Other/TransmissionSettings/UdpTransmissionItemWidget.ui \
    Src/Common/SAKDebugPage/Output/Save/SaveOutputDataSettings.ui \
    Src/HIDDebug/SAKHidDeviceController.ui \
    Src/SAKVersion.ui \
    Src/SAKMainWindow.ui \
    Src/MoreInformation.ui \
    Src/Console/SAKConsole.ui \
    Src/Common/SAKDebugPage/SAKDebugPage.ui \
    Src/Common/SAKDebugPage/Other/AutoResponseSettings/AutoResponseItemWidget.ui \
    Src/Common/SAKDebugPage/Other/ReadWriteSettings/ReadWriteSettingsWidget.ui \
    Src/Common/SAKDebugPage/Other/HighlightSettings/HighlightSettingsWidget.ui \
    Src/Common/SAKDebugPage/Other/AutoResponseSettings/AutoResponseSettingWidget.ui \
    Src/TcpClient/SAKTcpClientDeviceController.ui \
    Src/TcpServer/SAKTcpServerDeviceController.ui \
    Src/UdpDebug/SAKUdpDeviceController.ui


INCLUDEPATH += \
    Src \
    Src/Base \
    Src/Console \
    Src/Common/SAKDebugPage \
    Src/Common/SAKDebugPage/Other \
    Src/Common/SAKDebugPage/Other/HighlightSettings \
    Src/Common/SAKDebugPage/Other/AutoResponseSettings \
    Src/Common/SAKDebugPage/Other/ReadWriteSettings \
    Src/Common/SAKDebugPage/Other/TransmissionSettings \
    Src/Common/SAKDebugPage/Other/MoreOtherSettings \
    Src/Common/SAKDebugPage/Other/FormatSettings \
    Src/Common/SAKDebugPage/Statistics \
    Src/Common/SAKDebugPage/Chart \
    Src/Common/SAKDebugPage/Chart/Throughput \
    Src/Common/SAKDebugPage/Chart/MoreChart \
    Src/Common/SAKDebugPage/Output \
    Src/Common/SAKDebugPage/Output/Save \
    Src/Common/SAKDebugPage/Message \
    Src/Common/SAKDebugPage/Device \
    Src/Common/SAKDebugPage/Input

INCLUDEPATH += \
    Src/UdpDebug \
    Src/TcpClient \
    Src/TcpServer \
    Src/HidDebug

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
        Src/SerialPortDebug/SAKSerialPortDebugPage.cc \
        Src/SerialPortDebug/SAKSerialPortDeviceController.cc \
        Src/SerialPortDebug/SAKSerialPortDevice.cc
    HEADERS += \
        Src/SerialPortDebug/SAKSerialPortDebugPage.hh \
        Src/SerialPortDebug/SAKSerialPortDeviceController.hh \
        Src/SerialPortDebug/SAKSerialPortDevice.hh
    FORMS   += \
        Src/SerialPortDebug/SAKSerialPortDeviceController.ui
    INCLUDEPATH += \
        Src/SerialPortDebug
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

include(3rdParty/HidAPI/HidApi.pri)
include(Libs/LibUSB/LibUSB/LibUSB.pri)
