#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------

QT += core gui sql network

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

# Sub project
include(SAKConf.pri)
include(SAKSetup.pri)
include(src/tools/SAKTools.pri)
include(src/modbus/SAKModbus.pri)
include(platform/SAKPlatform.pri)
include(src/debuggers/SAKDebuggers.pri)

exists(private/SAKPrivate.pri){
    include(private/SAKPrivate.pri)
}

#------------------------------------------------------------------------------
# Android settings
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
    src/base \
    src/common \
    src/update

FORMS += \
    src/SAKMainWindow.ui \
    src/base/SAKBaseListWidget.ui \
    src/update/SAKDownloadItemWidget.ui \
    src/update/SAKUpdateManager.ui

HEADERS += \
    src/SAKApplication.hh \
    src/SAKMainWindow.hh \
    src/base/SAKBaseListWidget.hh \
    src/base/SAKBaseListWidgetItemWidget.hh \
    src/common/SAKCommonCrcInterface.hh \
    src/common/SAKCommonDataStructure.hh \
    src/common/SAKCommonInterface.hh \
    src/update/SAKDownloadItemWidget.hh \
    src/update/SAKUpdateManager.hh

SOURCES += \
    src/SAKApplication.cc \
    src/SAKMainWindow.cc \
    src/base/SAKBaseListWidget.cc \
    src/base/SAKBaseListWidgetItemWidget.cc \
    src/common/SAKCommonCrcInterface.cc \
    src/common/SAKCommonDataStructure.cc \
    src/common/SAKCommonInterface.cc \
    src/main.cc \
    src/update/SAKDownloadItemWidget.cc \
    src/update/SAKUpdateManager.cc
