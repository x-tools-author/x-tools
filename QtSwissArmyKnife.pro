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
include(SAKTcp.pri)
include(SAKUdp.pri)
include(SAKSetup.pri)
include(SAKTools.pri)
include(SAKCommon.pri)
include(SAKCharts.pri)
include(SAKModbus.pri)
include(SAKWebSocket.pri)
include(SAKSerialPort.pri)
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
    src/pages \
    src/pages/page \
    src/pages/page/common \
    src/pages/page/controller \
    src/pages/page/device \
    src/pages/page/device/mask \
    src/pages/page/input \
    src/pages/page/input/crcsettings \
    src/pages/page/input/datafactory \
    src/pages/page/input/datapreset \
    src/pages/page/other \
    src/pages/page/other/autoresponse \
    src/pages/page/other/highlighter \
    src/pages/page/other/generator \
    src/pages/page/other/analyzer \
    src/pages/page/other/timingsent \
    src/pages/page/other/transmission \
    src/pages/page/output \
    src/pages/page/output/save2file \
    src/pages/page/output/log \
    src/pages/page/statistics \
    src/pages/test \
    src/update

FORMS += \
    src/SAKMainWindow.ui \
    src/pages/page/SAKDebugPage.ui \
    src/pages/page/common/SAKDebugPageCommonSslConfigurationWidget.ui \
    src/pages/page/device/mask/SAKDebugPageDeviceMask.ui \
    src/pages/page/input/crcsettings/SAKInputCrcSettingsDialog.ui \
    src/pages/page/input/datapreset/SAKInputDataPresetItem.ui \
    src/pages/page/input/datapreset/SAKInputDataPresetItemManager.ui \
    src/pages/page/other/analyzer/SAKOtherAnalyzerThreadManager.ui \
    src/pages/page/other/autoresponse/SAKOtherAutoResponseItem.ui \
    src/pages/page/other/autoresponse/SAKOtherAutoResponseItemManager.ui \
    src/pages/page/other/highlighter/SAKOtherHighlighterManager.ui \
    src/pages/page/other/timingsent/SAKOtherTimingSentItem.ui \
    src/pages/page/other/timingsent/SAKOtherTimingSentItemManager.ui \
    src/pages/page/other/transmission/SAKOtherTransmissionItemCom.ui \
    src/pages/page/other/transmission/SAKOtherTransmissionItemTcp.ui \
    src/pages/page/other/transmission/SAKOtherTransmissionItemUdp.ui \
    src/pages/page/other/transmission/SAKOtherTransmissionPage.ui \
    src/pages/page/other/transmission/SAKOtherTransmissionPageViewer.ui \
    src/pages/page/output/log/SAKOutputLogDialog.ui \
    src/pages/page/output/save2file/SAKOutputSave2FileDialog.ui \
    src/pages/test/SAKTestDeviceController.ui \
    src/update/SAKDownloadItemWidget.ui \
    src/update/SAKUpdateManager.ui

HEADERS += \
    src/SAKApplication.hh \
    src/SAKMainWindow.hh \
    src/common/SAKCommonCrcInterface.hh \
    src/common/SAKCommonDataStructure.hh \
    src/common/SAKCommonInterface.hh \
    src/pages/page/SAKDebugPage.hh \
    src/pages/page/common/SAKDebugPageCommonDatabaseInterface.hh \
    src/pages/page/common/SAKDebugPageCommonSslConfigurationWidget.hh \
    src/pages/page/controller/SAKDebugPageController.hh \
    src/pages/page/device/SAKDebugPageDevice.hh \
    src/pages/page/device/mask/SAKDebugPageDeviceMask.hh \
    src/pages/page/input/SAKDebugPageInputController.hh \
    src/pages/page/input/crcsettings/SAKInputCrcSettingsDialog.hh \
    src/pages/page/input/datafactory/SAKInputDataFactory.hh \
    src/pages/page/input/datapreset/SAKInputDataPresetItem.hh \
    src/pages/page/input/datapreset/SAKInputDataPresetItemManager.hh \
    src/pages/page/other/SAKDebugPageOtherController.hh \
    src/pages/page/other/analyzer/SAKOtherAnalyzerThread.hh \
    src/pages/page/other/analyzer/SAKOtherAnalyzerThreadManager.hh \
    src/pages/page/other/autoresponse/SAKOtherAutoResponseItem.hh \
    src/pages/page/other/autoresponse/SAKOtherAutoResponseItemManager.hh \
    src/pages/page/other/highlighter/SAKOtherHighlighter.hh \
    src/pages/page/other/highlighter/SAKOtherHighlighterManager.hh \
    src/pages/page/other/timingsent/SAKOtherTimingSentItem.hh \
    src/pages/page/other/timingsent/SAKOtherTimingSentItemManager.hh \
    src/pages/page/other/transmission/SAKOtherTransmissionItem.hh \
    src/pages/page/other/transmission/SAKOtherTransmissionItemCom.hh \
    src/pages/page/other/transmission/SAKOtherTransmissionItemTcp.hh \
    src/pages/page/other/transmission/SAKOtherTransmissionItemUdp.hh \
    src/pages/page/other/transmission/SAKOtherTransmissionPage.hh \
    src/pages/page/other/transmission/SAKOtherTransmissionPageViewer.hh \
    src/pages/page/output/SAKDebugPageOutputController.hh \
    src/pages/page/output/log/SAKOutputLogDialog.hh \
    src/pages/page/output/save2file/SAKOutputSave2FileDialog.hh \
    src/pages/page/output/save2file/SAKOutputSave2FileThread.hh \
    src/pages/page/statistics/SAKDebugPageStatisticsController.hh \
    src/pages/test/SAKTestDebugPage.hh \
    src/pages/test/SAKTestDevice.hh \
    src/pages/test/SAKTestDeviceController.hh \
    src/update/SAKDownloadItemWidget.hh \
    src/update/SAKUpdateManager.hh

SOURCES += \
    src/SAKApplication.cc \
    src/SAKMainWindow.cc \
    src/common/SAKCommonCrcInterface.cc \
    src/common/SAKCommonDataStructure.cc \
    src/common/SAKCommonInterface.cc \
    src/pages/page/SAKDebugPage.cc \
    src/pages/page/common/SAKDebugPageCommonDatabaseInterface.cc \
    src/pages/page/common/SAKDebugPageCommonSslConfigurationWidget.cc \
    src/pages/page/controller/SAKDebugPageController.cc \
    src/pages/page/device/SAKDebugPageDevice.cc \
    src/pages/page/device/mask/SAKDebugPageDeviceMask.cc \
    src/pages/page/input/SAKDebugPageInputController.cc \
    src/pages/page/input/crcsettings/SAKInputCrcSettingsDialog.cc \
    src/pages/page/input/datafactory/SAKInputDataFactory.cc \
    src/pages/page/input/datapreset/SAKInputDataPresetItem.cc \
    src/pages/page/input/datapreset/SAKInputDataPresetItemManager.cc \
    src/pages/page/other/SAKDebugPageOtherController.cc \
    src/pages/page/other/analyzer/SAKOtherAnalyzerThread.cc \
    src/pages/page/other/analyzer/SAKOtherAnalyzerThreadManager.cc \
    src/pages/page/other/autoresponse/SAKOtherAutoResponseItem.cc \
    src/pages/page/other/autoresponse/SAKOtherAutoResponseItemManager.cc \
    src/pages/page/other/highlighter/SAKOtherHighlighter.cc \
    src/pages/page/other/highlighter/SAKOtherHighlighterManager.cc \
    src/pages/page/other/timingsent/SAKOtherTimingSentItem.cc \
    src/pages/page/other/timingsent/SAKOtherTimingSentItemManager.cc \
    src/pages/page/other/transmission/SAKOtherTransmissionItem.cc \
    src/pages/page/other/transmission/SAKOtherTransmissionItemCom.cc \
    src/pages/page/other/transmission/SAKOtherTransmissionItemTcp.cc \
    src/pages/page/other/transmission/SAKOtherTransmissionItemUdp.cc \
    src/pages/page/other/transmission/SAKOtherTransmissionPage.cc \
    src/pages/page/other/transmission/SAKOtherTransmissionPageViewer.cc \
    src/pages/page/output/SAKDebugPageOutputController.cc \
    src/pages/page/output/log/SAKOutputLogDialog.cc \
    src/pages/page/output/save2file/SAKOutputSave2FileDialog.cc \
    src/main.cc \
    src/pages/page/output/save2file/SAKOutputSave2FileThread.cc \
    src/pages/page/statistics/SAKDebugPageStatisticsController.cc \
    src/pages/test/SAKTestDebugPage.cc \
    src/pages/test/SAKTestDevice.cc \
    src/pages/test/SAKTestDeviceController.cc \
    src/update/SAKDownloadItemWidget.cc \
    src/update/SAKUpdateManager.cc
