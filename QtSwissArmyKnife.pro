#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------

QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

contains(CONFIG, debug, debug|release){
    TARGET = QtSwissArmyKnifed
}else{
    TARGET = QtSwissArmyKnife
}

win32:mingw {
    # It seems to be not effective
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++
}

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

# The file(SAKDefaultConfigure.pri) is use to control which module will be compiled.
# It is not suggested to modify the file directly.
# You should copy the file and rename it to SAKCustomConfigure.pri, then modify it.
exists($$PWD/SAKCustomConfigure.pri){
    include(SAKCustomConfigure.pri)
}else{
    include(SAKDefaultConfigure.pri)
}

#Sub project
include(src/debuggers/tcp/SAKTcp.pri)
include(SAKUdp.pri)
include(SAKSetup.pri)
include(SAKTools.pri)
include(SAKCommon.pri)
include(SAKCharts.pri)
include(SAKModbus.pri)
include(SAKWebSocket.pri)
include(src/debuggers/serialport/SAKSerialPort.pri)
include(SAKBluetoothLowEnergy.pri)

exists(private/SAKPrivate.pri){
    include(private/SAKPrivate.pri)
}

#------------------------------------------------------------------------------
# Android settings
# The package is qter.QtSwissArmyKnife
android {
    include(android/SAKAndroid.pri)
}

QSAK_APP_NAME        = "QtSwissArmyKnife"
QSAK_ORG_NAME        = "Qter"
QSAK_ORG_DOMAIN      = "IT"
QSAK_APP_DESCRIPTION = "Qt Swiss Army Knife"
QSAK_APP_COPYRIGHT   = "Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved."

win32 {
    QMAKE_TARGET_COMPANY        = "$${QSAK_ORG_NAME}"
    QMAKE_TARGET_DESCRIPTION    = "$${QSAK_APP_DESCRIPTION}"
    QMAKE_TARGET_COPYRIGHT      = "$${QSAK_APP_COPYRIGHT}"
    QMAKE_TARGET_PRODUCT        = "$${QSAK_APP_NAME}"
    QMAKE_TARGET_VERSION        = "$${SAK_VERSION}"
}

#--------------------------------------------------------------------------------------------
#Output directory
UI_DIR      = $$OUT_PWD/ui
MOC_DIR     = $$OUT_PWD/moc
RCC_DIR     = $$OUT_PWD/res
OBJECTS_DIR = $$OUT_PWD/obj

#--------------------------------------------------------------------------------------------
#Configuration of Windows
win32 {
    RC_ICONS = Windows.ico
    msvc:{
        lessThan(QT_MAJOR_VERSION, 6){
            QMAKE_CXXFLAGS += -execution-charset:utf-8
        }
    }

    include(libs/windows_openssl/SAKWindowsOpenSSL.pri)
}

#--------------------------------------------------------------------------------------------
#I18N
TRANSLATIONS  += \
    translations/sak/SAK_en.ts \
    translations/sak/SAK_zh_CN.ts

RESOURCES += \
    SAKResources.qrc

INCLUDEPATH += \
    src \
    src/common \
    src/debuggers \
    src/debuggers/debugger \
    src/debuggers/debugger/common \
    src/debuggers/debugger/controller \
    src/debuggers/debugger/device \
    src/debuggers/debugger/device/mask \
    src/debuggers/debugger/input \
    src/debuggers/debugger/input/crcsettings \
    src/debuggers/debugger/input/datafactory \
    src/debuggers/debugger/input/datapreset \
    src/debuggers/debugger/other \
    src/debuggers/debugger/other/autoresponse \
    src/debuggers/debugger/other/highlighter \
    src/debuggers/debugger/other/generator \
    src/debuggers/debugger/other/analyzer \
    src/debuggers/debugger/other/timingsent \
    src/debuggers/debugger/other/transmission \
    src/debuggers/debugger/output \
    src/debuggers/debugger/output/save2file \
    src/debuggers/debugger/output/log \
    src/debuggers/debugger/statistics \
    src/debuggers/test \
    src/update

FORMS += \
    src/SAKMainWindow.ui \
    src/debuggers/debugger/SAKDebugPage.ui \
    src/debuggers/debugger/common/SAKDebugPageCommonSslConfigurationWidget.ui \
    src/debuggers/debugger/device/mask/SAKDebugPageDeviceMask.ui \
    src/debuggers/debugger/input/crcsettings/SAKInputCrcSettingsDialog.ui \
    src/debuggers/debugger/input/datapreset/SAKInputDataPresetItem.ui \
    src/debuggers/debugger/input/datapreset/SAKInputDataPresetItemManager.ui \
    src/debuggers/debugger/other/analyzer/SAKOtherAnalyzerThreadManager.ui \
    src/debuggers/debugger/other/autoresponse/SAKOtherAutoResponseItem.ui \
    src/debuggers/debugger/other/autoresponse/SAKOtherAutoResponseItemManager.ui \
    src/debuggers/debugger/other/highlighter/SAKOtherHighlighterManager.ui \
    src/debuggers/debugger/other/timingsent/SAKOtherTimingSentItem.ui \
    src/debuggers/debugger/other/timingsent/SAKOtherTimingSentItemManager.ui \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItemCom.ui \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItemTcp.ui \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItemUdp.ui \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionPage.ui \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionPageViewer.ui \
    src/debuggers/debugger/output/log/SAKOutputLogDialog.ui \
    src/debuggers/debugger/output/save2file/SAKOutputSave2FileDialog.ui \
    src/debuggers/test/SAKTestDeviceController.ui \
    src/update/SAKDownloadItemWidget.ui \
    src/update/SAKUpdateManager.ui

HEADERS += \
    src/SAKApplication.hh \
    src/SAKMainWindow.hh \
    src/common/SAKCommonCrcInterface.hh \
    src/common/SAKCommonDataStructure.hh \
    src/common/SAKCommonInterface.hh \
    src/debuggers/debugger/SAKDebugPage.hh \
    src/debuggers/debugger/common/SAKDebugPageCommonDatabaseInterface.hh \
    src/debuggers/debugger/common/SAKDebugPageCommonSslConfigurationWidget.hh \
    src/debuggers/debugger/controller/SAKDebugPageController.hh \
    src/debuggers/debugger/device/SAKDebugPageDevice.hh \
    src/debuggers/debugger/device/mask/SAKDebugPageDeviceMask.hh \
    src/debuggers/debugger/input/SAKDebugPageInputController.hh \
    src/debuggers/debugger/input/crcsettings/SAKInputCrcSettingsDialog.hh \
    src/debuggers/debugger/input/datafactory/SAKInputDataFactory.hh \
    src/debuggers/debugger/input/datapreset/SAKInputDataPresetItem.hh \
    src/debuggers/debugger/input/datapreset/SAKInputDataPresetItemManager.hh \
    src/debuggers/debugger/other/SAKDebugPageOtherController.hh \
    src/debuggers/debugger/other/analyzer/SAKOtherAnalyzerThread.hh \
    src/debuggers/debugger/other/analyzer/SAKOtherAnalyzerThreadManager.hh \
    src/debuggers/debugger/other/autoresponse/SAKOtherAutoResponseItem.hh \
    src/debuggers/debugger/other/autoresponse/SAKOtherAutoResponseItemManager.hh \
    src/debuggers/debugger/other/highlighter/SAKOtherHighlighter.hh \
    src/debuggers/debugger/other/highlighter/SAKOtherHighlighterManager.hh \
    src/debuggers/debugger/other/timingsent/SAKOtherTimingSentItem.hh \
    src/debuggers/debugger/other/timingsent/SAKOtherTimingSentItemManager.hh \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItem.hh \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItemCom.hh \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItemTcp.hh \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItemUdp.hh \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionPage.hh \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionPageViewer.hh \
    src/debuggers/debugger/output/SAKDebugPageOutputController.hh \
    src/debuggers/debugger/output/log/SAKOutputLogDialog.hh \
    src/debuggers/debugger/output/save2file/SAKOutputSave2FileDialog.hh \
    src/debuggers/debugger/output/save2file/SAKOutputSave2FileThread.hh \
    src/debuggers/debugger/statistics/SAKDebugPageStatisticsController.hh \
    src/debuggers/test/SAKTestDebugPage.hh \
    src/debuggers/test/SAKTestDevice.hh \
    src/debuggers/test/SAKTestDeviceController.hh \
    src/update/SAKDownloadItemWidget.hh \
    src/update/SAKUpdateManager.hh

SOURCES += \
    src/SAKApplication.cc \
    src/SAKMainWindow.cc \
    src/common/SAKCommonCrcInterface.cc \
    src/common/SAKCommonDataStructure.cc \
    src/common/SAKCommonInterface.cc \
    src/debuggers/debugger/SAKDebugPage.cc \
    src/debuggers/debugger/common/SAKDebugPageCommonDatabaseInterface.cc \
    src/debuggers/debugger/common/SAKDebugPageCommonSslConfigurationWidget.cc \
    src/debuggers/debugger/controller/SAKDebugPageController.cc \
    src/debuggers/debugger/device/SAKDebugPageDevice.cc \
    src/debuggers/debugger/device/mask/SAKDebugPageDeviceMask.cc \
    src/debuggers/debugger/input/SAKDebugPageInputController.cc \
    src/debuggers/debugger/input/crcsettings/SAKInputCrcSettingsDialog.cc \
    src/debuggers/debugger/input/datafactory/SAKInputDataFactory.cc \
    src/debuggers/debugger/input/datapreset/SAKInputDataPresetItem.cc \
    src/debuggers/debugger/input/datapreset/SAKInputDataPresetItemManager.cc \
    src/debuggers/debugger/other/SAKDebugPageOtherController.cc \
    src/debuggers/debugger/other/analyzer/SAKOtherAnalyzerThread.cc \
    src/debuggers/debugger/other/analyzer/SAKOtherAnalyzerThreadManager.cc \
    src/debuggers/debugger/other/autoresponse/SAKOtherAutoResponseItem.cc \
    src/debuggers/debugger/other/autoresponse/SAKOtherAutoResponseItemManager.cc \
    src/debuggers/debugger/other/highlighter/SAKOtherHighlighter.cc \
    src/debuggers/debugger/other/highlighter/SAKOtherHighlighterManager.cc \
    src/debuggers/debugger/other/timingsent/SAKOtherTimingSentItem.cc \
    src/debuggers/debugger/other/timingsent/SAKOtherTimingSentItemManager.cc \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItem.cc \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItemCom.cc \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItemTcp.cc \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionItemUdp.cc \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionPage.cc \
    src/debuggers/debugger/other/transmission/SAKOtherTransmissionPageViewer.cc \
    src/debuggers/debugger/output/SAKDebugPageOutputController.cc \
    src/debuggers/debugger/output/log/SAKOutputLogDialog.cc \
    src/debuggers/debugger/output/save2file/SAKOutputSave2FileDialog.cc \
    src/main.cc \
    src/debuggers/debugger/output/save2file/SAKOutputSave2FileThread.cc \
    src/debuggers/debugger/statistics/SAKDebugPageStatisticsController.cc \
    src/debuggers/test/SAKTestDebugPage.cc \
    src/debuggers/test/SAKTestDevice.cc \
    src/debuggers/test/SAKTestDeviceController.cc \
    src/update/SAKDownloadItemWidget.cc \
    src/update/SAKUpdateManager.cc
