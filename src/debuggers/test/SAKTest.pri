DEFINES+=SAK_IMPORT_MODULE_TEST

contains(DEFINES, SAK_IMPORT_MODULE_TEST){
    FORMS += \
    $$PWD/SAKTestDebuggerController.ui

    HEADERS += \
    $$PWD/SAKTestDebugger.hh \
    $$PWD/SAKTestDebuggerController.hh \
    $$PWD/SAKTestDebuggerDevice.hh

    SOURCES += \
    $$PWD/SAKTestDebugger.cc \
    $$PWD/SAKTestDebuggerController.cc \
    $$PWD/SAKTestDebuggerDevice.cc

    INCLUDEPATH += \
        $$PWD
}
