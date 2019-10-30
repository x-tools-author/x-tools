QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += Src/Common

HEADERS += \
    Src/Common/SAKCRCInterface.hh

SOURCES += \
    Src/Common/SAKCRCInterface.cc \
    Test/SAKCRCInterfaceTest.cc

