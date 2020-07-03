INCLUDEPATH += \
    $$PWD/src/tools

HEADERS += \
    $$PWD/src/tools/SAKToolBase.hh \
    $$PWD/src/tools/SAKToolsManager.hh

SOURCES += \
    $$PWD/src/tools/SAKToolBase.cc \
    $$PWD/src/tools/SAKToolsManager.cc

include(src/tools/filechecker/SAKToolFileChecker.pri)
include(src/tools/qrcodecreator/SAKToolQRCodeCreator.pri)
include(src/tools/crccalculator/SAKToolCRCCalculator.pri)
