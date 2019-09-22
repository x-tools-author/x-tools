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

# 子项目
include(SAKCOM.pri)
include(SAKHID.pri)
include(SAKUSB.pri)
include(Modules.pri)


HEADERS += \
    Src/Base/SAKBase.hh \
    Src/Base/SAKCRCInterface.hh \
    Src/Base/SAKDialog.hh \
    Src/Base/SAKMessageBox.hh \
    Src/Base/SAKWidget.hh \
    Src/Console/SAKConsole.hh \
    Src/Console/SAKLogOutput.hh \
    Src/DebugPage/BaseDebugPage/Input/DebugPageInputManager.hh \
    Src/DebugPage/BaseDebugPage/Input/InputDataFactory.hh \
    Src/DebugPage/BaseDebugPage/Input/InputDataItem.hh \
    Src/DebugPage/BaseDebugPage/Other/AutoResponseSettings/AutoResponseItemWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/AutoResponseSettings/AutoResponseSettingWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/Chart/MoreChart/MoreChartWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/Chart/SAKChartManager.hh \
    Src/DebugPage/BaseDebugPage/Other/Chart/Throughput/ThroughputWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/FormatSettings/FormatSettingsWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/HighlightSettings/HighlightSettings.hh \
    Src/DebugPage/BaseDebugPage/Other/HighlightSettings/HighlightSettingsWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/MoreOtherSettings/MoreOtherSettingsWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/ReadWriteSettings/ReadWriteSettingsWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/SAKOtherSettings.hh \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/BaseTransmissionItemWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/SerialPortTransmissionItemWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TcpTransmissionItemWidget.hh \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TransmissionItemDelegate.hh \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TransmissionPage.hh \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TransmissionSettings.hh \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/UdpTransmissionItemWidget.hh \
    Src/DebugPage/BaseDebugPage/Output/DebugPageOutputManager.hh \
    Src/DebugPage/BaseDebugPage/Output/OutputDataFactory.hh \
    Src/DebugPage/BaseDebugPage/Output/Save/SaveOutputDataSettings.hh \
    Src/DebugPage/BaseDebugPage/Output/Save/SaveOutputDataThread.hh \
    Src/DebugPage/BaseDebugPage/SAKDebugPage.hh \
    Src/DebugPage/BaseDebugPage/Statistics/SAKStatisticsManager.hh \
    Src/DebugPage/TcpClient/SAKTcpClientDebugPage.hh \
    Src/DebugPage/TcpClient/SAKTcpClientDevice.hh \
    Src/DebugPage/TcpClient/SAKTcpClientDeviceController.hh \
    Src/DebugPage/TcpServer/SAKTcpServerDebugPage.hh \
    Src/DebugPage/TcpServer/SAKTcpServerDevice.hh \
    Src/DebugPage/TcpServer/SAKTcpServerDeviceController.hh \
    Src/DebugPage/UdpDebug/SAKUdpDebugPage.hh \
    Src/DebugPage/UdpDebug/SAKUdpDevice.hh \
    Src/DebugPage/UdpDebug/SAKUdpDeviceController.hh \
    Src/MoreInformation.hh \
    Src/SAKApplication.hh \
    Src/SAKGlobal.hh \
    Src/SAKMainWindow.hh \
    Src/SAKVersion.hh \
    Src/Tools/SAKBaseTool/SAKToolDialog.hh


SOURCES += \
    Src/Base/SAKBase.cc \
    Src/Base/SAKCRCInterface.cc \
    Src/Base/SAKDialog.cc \
    Src/Base/SAKMessageBox.cc \
    Src/Base/SAKWidget.cc \
    Src/Console/SAKConsole.cc \
    Src/Console/SAKLogOutput.cc \
    Src/DebugPage/BaseDebugPage/Input/DebugPageInputManager.cc \
    Src/DebugPage/BaseDebugPage/Input/InputDataFactory.cc \
    Src/DebugPage/BaseDebugPage/Input/InputDataItem.cc \
    Src/DebugPage/BaseDebugPage/Other/AutoResponseSettings/AutoResponseItemWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/AutoResponseSettings/AutoResponseSettingWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/Chart/MoreChart/MoreChartWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/Chart/SAKChartManager.cc \
    Src/DebugPage/BaseDebugPage/Other/Chart/Throughput/ThroughputWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/FormatSettings/FormatSettingsWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/HighlightSettings/HighlightSettings.cc \
    Src/DebugPage/BaseDebugPage/Other/HighlightSettings/HighlightSettingsWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/MoreOtherSettings/MoreOtherSettingsWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/ReadWriteSettings/ReadWriteSettingsWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/SAKOtherSettings.cc \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/BaseTransmissionItemWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/SerialPortTransmissionItemWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TcpTransmissionItemWidget.cc \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TransmissionItemDelegate.cc \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TransmissionPage.cc \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TransmissionSettings.cc \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/UdpTransmissionItemWidget.cc \
    Src/DebugPage/BaseDebugPage/Output/DebugPageOutputManager.cc \
    Src/DebugPage/BaseDebugPage/Output/OutputDataFactory.cc \
    Src/DebugPage/BaseDebugPage/Output/Save/SaveOutputDataSettings.cc \
    Src/DebugPage/BaseDebugPage/Output/Save/SaveOutputDataThread.cc \
    Src/DebugPage/BaseDebugPage/SAKDebugPage.cc \
    Src/DebugPage/BaseDebugPage/Statistics/SAKStatisticsManager.cc \
    Src/DebugPage/TcpClient/SAKTcpClientDebugPage.cc \
    Src/DebugPage/TcpClient/SAKTcpClientDevice.cc \
    Src/DebugPage/TcpClient/SAKTcpClientDeviceController.cc \
    Src/DebugPage/TcpServer/SAKTcpServerDebugPage.cc \
    Src/DebugPage/TcpServer/SAKTcpServerDevice.cc \
    Src/DebugPage/TcpServer/SAKTcpServerDeviceController.cc \
    Src/DebugPage/UdpDebug/SAKUdpDebugPage.cc \
    Src/DebugPage/UdpDebug/SAKUdpDevice.cc \
    Src/DebugPage/UdpDebug/SAKUdpDeviceController.cc \    
    Src/MoreInformation.cc \
    Src/SAKApplication.cc \
    Src/SAKGlobal.cc \
    Src/SAKMainWindow.cc \
    Src/SAKVersion.cc \
    Src/Tools/SAKBaseTool/SAKToolDialog.cc \
    Src/main.cc


FORMS += \
    Src/Console/SAKConsole.ui \
    Src/DebugPage/BaseDebugPage/Other/Chart/MoreChart/MoreChartWidget.ui \
    Src/DebugPage/BaseDebugPage/Input/InputDataItem.ui \
    Src/DebugPage/BaseDebugPage/Other/AutoResponseSettings/AutoResponseItemWidget.ui \
    Src/DebugPage/BaseDebugPage/Other/AutoResponseSettings/AutoResponseSettingWidget.ui \
    Src/DebugPage/BaseDebugPage/Other/FormatSettings/FormatSettingsWidget.ui \
    Src/DebugPage/BaseDebugPage/Other/HighlightSettings/HighlightSettingsWidget.ui \
    Src/DebugPage/BaseDebugPage/Other/MoreOtherSettings/MoreOtherSettingsWidget.ui \
    Src/DebugPage/BaseDebugPage/Other/ReadWriteSettings/ReadWriteSettingsWidget.ui \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/SerialPortTransmissionItemWidget.ui \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TcpTransmissionItemWidget.ui \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TransmissionPage.ui \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/TransmissionSettings.ui \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings/UdpTransmissionItemWidget.ui \
    Src/DebugPage/BaseDebugPage/Output/Save/SaveOutputDataSettings.ui \
    Src/DebugPage/BaseDebugPage/SAKDebugPage.ui \
    Src/DebugPage/TcpClient/SAKTcpClientDeviceController.ui \
    Src/DebugPage/TcpServer/SAKTcpServerDeviceController.ui \
    Src/DebugPage/UdpDebug/SAKUdpDeviceController.ui \    
    Src/MoreInformation.ui \
    Src/SAKMainWindow.ui \
    Src/SAKVersion.ui



INCLUDEPATH += \
    Src \
    Src/Base \
    Src/Console \
    Src/DebugPage/BaseDebugPage \
    Src/DebugPage/BaseDebugPage/Other \
    Src/DebugPage/BaseDebugPage/Other/HighlightSettings \
    Src/DebugPage/BaseDebugPage/Other/AutoResponseSettings \
    Src/DebugPage/BaseDebugPage/Other/ReadWriteSettings \
    Src/DebugPage/BaseDebugPage/Other/TransmissionSettings \
    Src/DebugPage/BaseDebugPage/Other/MoreOtherSettings \
    Src/DebugPage/BaseDebugPage/Other/FormatSettings \
    Src/DebugPage/BaseDebugPage/Statistics \
    Src/DebugPage/BaseDebugPage/Other/Chart \
    Src/DebugPage/BaseDebugPage/Other/Chart/Throughput \
    Src/DebugPage/BaseDebugPage/Other/Chart/MoreChart \
    Src/DebugPage/BaseDebugPage/Output \
    Src/DebugPage/BaseDebugPage/Output/Save \
    Src/DebugPage/BaseDebugPage/Message \
    Src/DebugPage/BaseDebugPage/Device \
    Src/DebugPage/BaseDebugPage/Input


INCLUDEPATH += \
    Src/DebugPage/UdpDebug \
    Src/DebugPage/TcpClient \
    Src/DebugPage/TcpServer


RESOURCES += \
    SAKResources.qrc

#--------------------------------------------------------------------------------------------
#编译目录配置
UI_DIR      = $$OUT_PWD/ui
MOC_DIR     = $$OUT_PWD/moc
RCC_DIR     = $$OUT_PWD/res
OBJECTS_DIR = $$OUT_PWD/obj

#--------------------------------------------------------------------------------------------
#Windows配置
win32 {
    RC_ICONS = Windows.ico
    msvc:{
        QMAKE_CXXFLAGS += -execution-charset:utf-8
    }
}
