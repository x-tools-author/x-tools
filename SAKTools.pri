INCLUDEPATH += \
    $$PWD/src/tools

HEADERS += \
    $$PWD/src/tools/SAKToolBase.hh \
    $$PWD/src/tools/SAKToolsManager.hh

SOURCES += \
    $$PWD/src/tools/SAKToolBase.cc \
    $$PWD/src/tools/SAKToolsManager.cc

include(src/tools/qrencode/SAKQRCode.pri)
include(src/tools/filechecker/SAKFileChecker.pri)
include(src/tools/crccalculator/SAKCRCCalculator.pri)
