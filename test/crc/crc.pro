QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../src/common \

SOURCES += \
    ../../src/common/SAKCommonCrcInterface.cc \
    SAKCRCInterfaceTest.cc

HEADERS += \
    ../../src/common/SAKCommonCrcInterface.hh
