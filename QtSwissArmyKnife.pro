#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------

QT += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# 子项目
include(SAKSetup.pri)
include(SAKTools.pri)
include(SAKDataVis.pri)
include(SAKModules.pri)
include(SAKSerialPort.pri)

exists(private/SAKPrivate.pri){
    include(private/SAKPrivate.pri)
}

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

#--------------------------------------------------------------------------------------------
#国际化文件
TRANSLATIONS  += \
    Translations/sak/SAK_en.ts \
    Translations/sak/SAK_zh_CN.ts \
    Translations/sak/SAK_zh_TW.ts

RESOURCES += \
    SAKResources.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

INCLUDEPATH += \
    src \
    src/base \
    src/common \
    src/qrcode \
    src/base/page \
    src/base/page/input \
    src/base/page/other \
    src/base/page/other/autoresponse \
    src/base/page/other/highlight \
    src/base/page/other/more \
    src/base/page/other/readwrite \
    src/base/page/other/timing \
    src/base/page/other/transmission \
    src/base/page/output \
    src/base/page/output/save \
    src/base/page/statistics \
    src/pages/tcpclient \
    src/pages/tcpserver \
    src/pages/udp \
    src/update

FORMS += \
    src/SAKMainWindow.ui \
    src/SAKMoreInformation.ui \
    src/base/page/SAKDebugPage.ui \
    src/base/page/input/SAKInputDataItem.ui \
    src/base/page/input/SAKInputDataItemManager.ui \
    src/base/page/other/autoresponse/SAKAutoResponseItemWidget.ui \
    src/base/page/other/autoresponse/SAKAutoResponseSettingsWidget.ui \
    src/base/page/other/highlight/SAKHighlightSettingsWidget.ui \
    src/base/page/other/more/SAKMoreSettingsWidget.ui \
    src/base/page/other/readwrite/SAKReadWriteSettingsWidget.ui \
    src/base/page/other/timing/SAKTimingSendingItemWidget.ui \
    src/base/page/other/timing/SAKTimingSendingSettingsWidget.ui \
    src/base/page/other/transmission/SAKSerialPortTransmissionItemWidget.ui \
    src/base/page/other/transmission/SAKTcpTransmissionItemWidget.ui \
    src/base/page/other/transmission/SAKTransmissionPage.ui \
    src/base/page/other/transmission/SAKTransmissionSettings.ui \
    src/base/page/other/transmission/SAKUdpTransmissionItemWidget.ui \
    src/base/page/output/save/SAKSaveOutputDataSettings.ui \
    src/pages/tcpclient/SAKTcpClientDeviceController.ui \
    src/pages/tcpserver/SAKTcpServerDeviceController.ui \
    src/pages/udp/SAKUdpAdvanceSettingWidget.ui \
    src/pages/udp/SAKUdpDeviceController.ui \
    src/pages/udp/SAKUdpMulticastEditingDialog.ui \
    src/qrcode/SAKQRCodeDialog.ui \
    src/update/SAKDownloadItemWidget.ui \
    src/update/SAKUpdateManager.ui

HEADERS += \
    src/SAKApplication.hh \
    src/SAKApplicationInformation.hh \
    src/SAKCodingStyle.hh \
    src/SAKGlobal.hh \
    src/SAKMainWindow.hh \
    src/SAKMoreInformation.hh \
    src/SAKSettings.hh \
    src/base/SAKDialog.hh \
    src/base/SAKMessageBox.hh \
    src/base/SAKWidget.hh \
    src/base/page/SAKDebugPage.hh \
    src/base/page/input/SAKDebugPageInputManager.hh \
    src/base/page/input/SAKInputDataFactory.hh \
    src/base/page/input/SAKInputDataItem.hh \
    src/base/page/input/SAKInputDataItemManager.hh \
    src/base/page/other/SAKOtherSettingsManager.hh \
    src/base/page/other/autoresponse/SAKAutoResponseItemWidget.hh \
    src/base/page/other/autoresponse/SAKAutoResponseSettingsWidget.hh \
    src/base/page/other/highlight/SAKHighlightSettings.hh \
    src/base/page/other/highlight/SAKHighlightSettingsWidget.hh \
    src/base/page/other/more/SAKMoreSettingsWidget.hh \
    src/base/page/other/readwrite/SAKReadWriteSettingsWidget.hh \
    src/base/page/other/timing/SAKTimingSendingItemWidget.hh \
    src/base/page/other/timing/SAKTimingSendingSettingsWidget.hh \
    src/base/page/other/transmission/SAKBaseTransmissionItemWidget.hh \
    src/base/page/other/transmission/SAKSerialPortTransmissionItemWidget.hh \
    src/base/page/other/transmission/SAKTcpTransmissionItemWidget.hh \
    src/base/page/other/transmission/SAKTransmissionItemDelegate.hh \
    src/base/page/other/transmission/SAKTransmissionPage.hh \
    src/base/page/other/transmission/SAKTransmissionSettings.hh \
    src/base/page/other/transmission/SAKUdpTransmissionItemWidget.hh \
    src/base/page/output/SAKDebugPageOutputManager.hh \
    src/base/page/output/SAKOutputDataFactory.hh \
    src/base/page/output/save/SAKSaveOutputDataSettings.hh \
    src/base/page/output/save/SAKSaveOutputDataThread.hh \
    src/base/page/statistics/SAKStatisticsManager.hh \
    src/common/SAKCRCInterface.hh \
    src/common/SAKCommonInterface.hh \
    src/common/SAKDataStruct.hh \
    src/pages/tcpclient/SAKTcpClientDebugPage.hh \
    src/pages/tcpclient/SAKTcpClientDevice.hh \
    src/pages/tcpclient/SAKTcpClientDeviceController.hh \
    src/pages/tcpserver/SAKTcpServerDebugPage.hh \
    src/pages/tcpserver/SAKTcpServerDevice.hh \
    src/pages/tcpserver/SAKTcpServerDeviceController.hh \
    src/pages/udp/SAKUdpAdvanceSettingWidget.hh \
    src/pages/udp/SAKUdpDebugPage.hh \
    src/pages/udp/SAKUdpDevice.hh \
    src/pages/udp/SAKUdpDeviceController.hh \
    src/pages/udp/SAKUdpMulticastEditingDialog.hh \
    src/qrcode/SAKQRCodeDialog.hh \
    src/qrcode/SAKQRCodeWidget.hh \
    src/update/SAKDownloadItemWidget.hh \
    src/update/SAKUpdateManager.hh

SOURCES += \
    src/SAKApplication.cc \
    src/SAKApplicationInformation.cc \
    src/SAKCodingStyle.cc \
    src/SAKGlobal.cc \
    src/SAKMainWindow.cc \
    src/SAKMoreInformation.cc \
    src/SAKSettings.cc \
    src/base/SAKDialog.cc \
    src/base/SAKMessageBox.cc \
    src/base/SAKWidget.cc \
    src/base/page/SAKDebugPage.cc \
    src/base/page/input/SAKDebugPageInputManager.cc \
    src/base/page/input/SAKInputDataFactory.cc \
    src/base/page/input/SAKInputDataItem.cc \
    src/base/page/input/SAKInputDataItemManager.cc \
    src/base/page/other/SAKOtherSettingsManager.cc \
    src/base/page/other/autoresponse/SAKAutoResponseItemWidget.cc \
    src/base/page/other/autoresponse/SAKAutoResponseSettingsWidget.cc \
    src/base/page/other/highlight/SAKHighlightSettings.cc \
    src/base/page/other/highlight/SAKHighlightSettingsWidget.cc \
    src/base/page/other/more/SAKMoreSettingsWidget.cc \
    src/base/page/other/readwrite/SAKReadWriteSettingsWidget.cc \
    src/base/page/other/timing/SAKTimingSendingItemWidget.cc \
    src/base/page/other/timing/SAKTimingSendingSettingsWidget.cc \
    src/base/page/other/transmission/SAKBaseTransmissionItemWidget.cc \
    src/base/page/other/transmission/SAKSerialPortTransmissionItemWidget.cc \
    src/base/page/other/transmission/SAKTcpTransmissionItemWidget.cc \
    src/base/page/other/transmission/SAKTransmissionItemDelegate.cc \
    src/base/page/other/transmission/SAKTransmissionPage.cc \
    src/base/page/other/transmission/SAKTransmissionSettings.cc \
    src/base/page/other/transmission/SAKUdpTransmissionItemWidget.cc \
    src/base/page/output/SAKDebugPageOutputManager.cc \
    src/base/page/output/SAKOutputDataFactory.cc \
    src/base/page/output/save/SAKSaveOutputDataSettings.cc \
    src/base/page/output/save/SAKSaveOutputDataThread.cc \
    src/base/page/statistics/SAKStatisticsManager.cc \
    src/common/SAKCRCInterface.cc \
    src/common/SAKCommonInterface.cc \
    src/common/SAKDataStruct.cc \
    src/main.cc \
    src/pages/tcpclient/SAKTcpClientDebugPage.cc \
    src/pages/tcpclient/SAKTcpClientDevice.cc \
    src/pages/tcpclient/SAKTcpClientDeviceController.cc \
    src/pages/tcpserver/SAKTcpServerDebugPage.cc \
    src/pages/tcpserver/SAKTcpServerDevice.cc \
    src/pages/tcpserver/SAKTcpServerDeviceController.cc \
    src/pages/udp/SAKUdpAdvanceSettingWidget.cc \
    src/pages/udp/SAKUdpDebugPage.cc \
    src/pages/udp/SAKUdpDevice.cc \
    src/pages/udp/SAKUdpDeviceController.cc \
    src/pages/udp/SAKUdpMulticastEditingDialog.cc \
    src/qrcode/SAKQRCodeDialog.cc \
    src/qrcode/SAKQRCodeWidget.cc \
    src/update/SAKDownloadItemWidget.cc \
    src/update/SAKUpdateManager.cc
