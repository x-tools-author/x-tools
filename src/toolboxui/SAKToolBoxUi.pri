INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/base

FORMS += \
    $$PWD/base/SAKToolBoxUi.ui \
    $$PWD/base/SAKToolBoxUiParameters.ui

HEADERS += \
    $$PWD/SAKToolBoxUiFactory.hh \
    $$PWD/base/SAKToolBoxUi.hh \
    $$PWD/base/SAKToolBoxUiController.hh \
    $$PWD/base/SAKToolBoxUiParameters.hh

SOURCES += \
    $$PWD/SAKToolBoxUiFactory.cc \
    $$PWD/base/SAKToolBoxUi.cc \
    $$PWD/base/SAKToolBoxUiController.cc \
    $$PWD/base/SAKToolBoxUiParameters.cc

include(serialport/SAKSerialPortToolBoxUi.pri)
