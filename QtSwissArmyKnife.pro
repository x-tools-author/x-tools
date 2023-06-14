#-------------------------------------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------------------------------------

QT += core gui network bluetooth serialport websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

contains(CONFIG, debug, debug|release) {
    TARGET = QtSwissArmyKnifed
} else {
    TARGET = QtSwissArmyKnife
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

#------------------------------------------------------------------------------
# Sub project, take cate the including order
include(SAKProject.pri)
include(src/common/SAKCommon.pri)
include(src/commonui/SAKCommonUi.pri)

include(src/log/SAKLog.pri)
include(src/logui/SAKLogUi.pri)
include(src/tools/SAKTools.pri)
include(src/toolsui/SAKToolsUi.pri)
include(src/toolbox/SAKToolBox.pri)
include(src/toolboxui/SAKToolBoxUi.pri)
include(src/preferences/SAKPreferences.pri)
include(src/preferencesui/SAKPreferencesUi.pri)

include(src/assistants/SAKAssistants.pri)
include(src/assistantsui/SAKAssistantsUi.pri)

include(src/canbus/SAKCanBus.pri)
include(src/canbusui/SAKCanBusUi.pri)

include(src/modbus/SAKModbus.pri)
include(src/modbusui/SAKModbusUi.pri)

#------------------------------------------------------------------------------
# Android settings
QSAK_APP_NAME        = "QtSwissArmyKnife"
QSAK_ORG_NAME        = "Qsaker"
QSAK_ORG_DOMAIN      = "IT"
QSAK_APP_DESCRIPTION = "Qt Swiss Army Knife"
QSAK_APP_COPYRIGHT   = "Copyright 2018-2023 Qsaker(wuuhaii@outlook.com). All rights reserved."

win32 {
    QMAKE_TARGET_COMPANY        = "$${QSAK_ORG_NAME}"
    QMAKE_TARGET_DESCRIPTION    = "$${QSAK_APP_DESCRIPTION}"
    QMAKE_TARGET_COPYRIGHT      = "$${QSAK_APP_COPYRIGHT}"
    QMAKE_TARGET_PRODUCT        = "$${QSAK_APP_NAME}"
    QMAKE_TARGET_VERSION        = "$${SAK_VERSION}"
}

#-------------------------------------------------------------------------------
#Output directory
UI_DIR      = $$OUT_PWD/ui
MOC_DIR     = $$OUT_PWD/moc
RCC_DIR     = $$OUT_PWD/res
OBJECTS_DIR = $$OUT_PWD/obj

#-------------------------------------------------------------------------------
#Configuration of Windows
win32 {
    RC_ICONS = Windows.ico
    msvc {
        lessThan(QT_MAJOR_VERSION, 6) {
            QMAKE_CXXFLAGS += -execution-charset:utf-8
        }
    }

    mingw {
        QMAKE_LFLAGS += -static
    }
}

#-------------------------------------------------------------------------------
#I18N
TRANSLATIONS  += \
    resources/translations/sak_en.ts \
    resources/translations/sak_zh_CN.ts \
    resources/translations/sak_ar.ts \
    resources/translations/sak_cs.ts \
    resources/translations/sak_da.ts \
    resources/translations/sak_de.ts \
    resources/translations/sak_en.ts \
    resources/translations/sak_es.ts \
    resources/translations/sak_fa.ts \
    resources/translations/sak_fi.ts \
    resources/translations/sak_fr.ts \
    resources/translations/sak_he.ts \
    resources/translations/sak_uk.ts \
    resources/translations/sak_it.ts \
    resources/translations/sak_ja.ts \
    resources/translations/sak_ko.ts \
    resources/translations/sak_lt.ts \
    resources/translations/sak_pl.ts \
    resources/translations/sak_pt.ts \
    resources/translations/sak_ru.ts \
    resources/translations/sak_sk.ts \
    resources/translations/sak_sl.ts \
    resources/translations/sak_sv.ts

RESOURCES += \
    SAKResources.qrc

INCLUDEPATH += \
    src

FORMS += \
    src/SAKMainWindow.ui

HEADERS += \
    src/SAKApplication.hh \
    src/SAKMainWindow.hh \
    src/SAKSystemTrayIcon.hh

SOURCES += \
    src/SAKApplication.cc \
    src/SAKMainWindow.cc \
    src/SAKSystemTrayIcon.cc \
    src/main.cc

greaterThan(QT_MAJOR_VERSION, 5) {
    greaterThan(QT_MINOR_VERSION, 3) {
        QT += quick quickcontrols2

        HEADERS += src/SAKGuiApplication.hh
        SOURCES += src/SAKGuiApplication.cc
        RESOURCES += SAKQmlResources.qrc
    }
} else {
    message("If you want to using QML UI, you must use Qt6.4.0 or later.")
}
