DEFINES += SAK_IMPORT_MODULE_BLE_CENTRAL
INCLUDEPATH += $$PWD

FORMS += \
    $$PWD/SAKBleCentralController.ui

HEADERS += \
    $$PWD/SAKBleCentralController.hh \
    $$PWD/SAKBleCentralDebugger.hh \
    $$PWD/SAKBleCentralDevice.hh

SOURCES += \
    $$PWD/SAKBleCentralController.cc \
    $$PWD/SAKBleCentralDebugger.cc \
    $$PWD/SAKBleCentralDevice.cc
