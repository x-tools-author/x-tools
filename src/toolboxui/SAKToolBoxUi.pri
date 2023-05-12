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


#------------------------------------------------------------------------------
include(ble/SAKBleToolBoxUi.pri)
include(tcp/SAKTcpToolBoxUi.pri)
include(udp/SAKUdpToolBoxUi.pri)
include(websocket/SAKWebSocketToolBoxUi.pri)
include(serialport/SAKSerialPortToolBoxUi.pri)
