INCLUDEPATH += \
    src \
    src/base \
    src/common

HEADERS += \
    $$PWD/src/SAKApplication.hh \
    $$PWD/src/SAKApplicationInformation.hh \
    $$PWD/src/SAKCodingStyle.hh \
    $$PWD/src/SAKGlobal.hh \
    $$PWD/src/SAKSettings.hh \
    $$PWD/src/common/SAKCRCInterface.hh \
    $$PWD/src/common/SAKCommonInterface.hh \
    $$PWD/src/common/SAKDataStruct.hh

SOURCES += \
    $$PWD/src/SAKApplication.cc \
    $$PWD/src/SAKApplicationInformation.cc \
    $$PWD/src/SAKCodingStyle.cc \
    $$PWD/src/SAKGlobal.cc \
    $$PWD/src/SAKSettings.cc \
    $$PWD/src/common/SAKCRCInterface.cc \
    $$PWD/src/common/SAKCommonInterface.cc \
    $$PWD/src/common/SAKDataStruct.cc \
    $$PWD/src/main.cc
