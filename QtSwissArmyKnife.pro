#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------

QT += core gui xml charts network

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

# 子项目
include(SAKCOM.pri)
include(Modules.pri)
include(SAKSetup.pri)

#--------------------------------------------------------------------------------------------
#编译目录配置
UI_DIR      = $$OUT_PWD/ui
MOC_DIR     = $$OUT_PWD/moc
RCC_DIR     = $$OUT_PWD/res
OBJECTS_DIR = $$OUT_PWD/obj


#--------------------------------------------------------------------------------------------
#Windows配置
win32 {
    msvc:{
        QMAKE_CXXFLAGS += -execution-charset:utf-8
    }
}


#--------------------------------------------------------------------------------------------
#国际化文件
TRANSLATIONS  += \
    Translations/SAK_en.ts \
    Translations/SAK_zh_CN.ts \
    Translations/SAK_zh_TW.ts


RESOURCES += \
    SAKResources.qrc


RC_ICONS = Windows.ico


HEADERS +=


SOURCES +=


FORMS +=


INCLUDEPATH += \
    Src \
    Src/Base
