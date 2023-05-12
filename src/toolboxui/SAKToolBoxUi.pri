include(serialport/SAKToolBoxUiSerialPort.pri)

HEADERS += \
    $$PWD/SAKToolBoxUi.hh \
    $$PWD/SAKToolBoxUiBaseController.hh \
    $$PWD/SAKToolBoxUiControllerFactory.hh \
    $$PWD/SAKToolBoxUiFactory.hh \
    $$PWD/SAKToolBoxUiParameters.hh \
    $$PWD/SAKToolBoxUiSerialPortController.hh

SOURCES += \
    $$PWD/SAKToolBoxUi.cc \
    $$PWD/SAKToolBoxUiBaseController.cc \
    $$PWD/SAKToolBoxUiControllerFactory.cc \
    $$PWD/SAKToolBoxUiFactory.cc \
    $$PWD/SAKToolBoxUiParameters.cc \
    $$PWD/SAKToolBoxUiSerialPortController.cc

FORMS += \
    $$PWD/SAKToolBoxUi.ui \
    $$PWD/SAKToolBoxUiParameters.ui \
    $$PWD/SAKToolBoxUiSerialPortController.ui

