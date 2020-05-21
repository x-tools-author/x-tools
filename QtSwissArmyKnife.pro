#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T22:57:33
#
#-------------------------------------------------

QT += core gui widgets qml

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

#--------------------------------------------------------------------------------------------
#工程仅适用于qt5.12
equals(QT_MAJOR_VERSION, 5){
    !equals(QT_MINOR_VERSION, 12){
        error("The project is for Qt5.12 only!")
    }
}else{
    error("The project is for Qt5 only!")
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

#--------------------------------------------------------------------------------------------
#安卓配置
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

#--------------------------------------------------------------------------------------------
#子工程
include(SAKSetup.pri)
include(SAKSerialport.pri)

#--------------------------------------------------------------------------------------------
#资源文件
RESOURCES += \
    SAKResources.qrc

#--------------------------------------------------------------------------------------------
#头文件目录
INCLUDEPATH += \
    src/common \
    src/console \
    src/debugger \
    src/debugger/device \
    src/debugger/input \
    src/debugger/input/settings \
    src/debugger/input/textinput \
    src/debugger/output \
    src/debugger/output/settings \
    src/debugger/output/textoutput \
    src/help \
    src/update

#--------------------------------------------------------------------------------------------
#源码
HEADERS += \
    src/SAKApplication.hh \
    src/SAKDebuggerManager.hh \
    src/common/SAKCRCInterface.hh \
    src/console/SAKConsoleManager.hh \
    src/console/SAKConsoleMessage.hh \
    src/debugger/SAKDebugger.hh \
    src/debugger/device/SAKDebuggerDevice.hh \
    src/debugger/input/SAKDebuggerInputManager.hh \
    src/debugger/input/settings/SAKDebuggerInputSettings.hh \
    src/debugger/input/textinput/SAKDebuggerInputTextDataPresetController.hh \
    src/debugger/input/textinput/SAKDebuggerInputTextDataPresetItem.hh \
    src/debugger/input/textinput/SAKDebuggerInputTextFactory.hh \
    src/debugger/input/textinput/SAKDebuggerTextInput.hh \
    src/debugger/output/SAKDebuggerOutputManager.hh \
    src/debugger/output/settings/SAKDebuggerOutputSettings.hh \
    src/debugger/output/settings/SAKDebuggerOutputStatistics.hh \
    src/debugger/output/textoutput/SAKDebuggerOutputTextFactory.hh \
    src/debugger/output/textoutput/SAKDebuggerTextOutput.hh \
    src/help/SAKHelp3rdController.hh \
    src/help/SAKHelpAboutQtController.hh \
    src/help/SAKHelpAboutSAKController.hh \
    src/help/SAKHelpBuyController.hh \
    src/help/SAKHelpDonationController.hh \
    src/help/SAKHelpHistoryController.hh \
    src/help/SAKHelpManager.hh \
    src/update/SAKUpdateManager.hh

SOURCES += \
    src/SAKApplication.cc \
    src/SAKDebuggerManager.cc \
    src/common/SAKCRCInterface.cc \
    src/console/SAKConsoleManager.cc \
    src/console/SAKConsoleMessage.cc \
    src/debugger/SAKDebugger.cc \
    src/debugger/device/SAKDebuggerDevice.cc \
    src/debugger/input/SAKDebuggerInputManager.cc \
    src/debugger/input/settings/SAKDebuggerInputSettings.cc \
    src/debugger/input/textinput/SAKDebuggerInputTextDataPresetController.cc \
    src/debugger/input/textinput/SAKDebuggerInputTextDataPresetItem.cc \
    src/debugger/input/textinput/SAKDebuggerInputTextFactory.cc \
    src/debugger/input/textinput/SAKDebuggerTextInput.cc \
    src/debugger/output/SAKDebuggerOutputManager.cc \
    src/debugger/output/settings/SAKDebuggerOutputSettings.cc \
    src/debugger/output/settings/SAKDebuggerOutputStatistics.cc \
    src/debugger/output/textoutput/SAKDebuggerOutputTextFactory.cc \
    src/debugger/output/textoutput/SAKDebuggerTextOutput.cc \
    src/help/SAKHelp3rdController.cc \
    src/help/SAKHelpAboutQtController.cc \
    src/help/SAKHelpAboutSAKController.cc \
    src/help/SAKHelpBuyController.cc \
    src/help/SAKHelpDonationController.cc \
    src/help/SAKHelpHistoryController.cc \
    src/help/SAKHelpManager.cc \
    src/main.cc \
    src/update/SAKUpdateManager.cc
