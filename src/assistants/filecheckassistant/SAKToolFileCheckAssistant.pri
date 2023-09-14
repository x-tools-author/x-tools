# 该模块使用了5.9版本中引入的新数据类型
DEFINES+=SAK_IMPORT_MODULE_FILECHECKASSISTANT

contains(DEFINES, SAK_IMPORT_MODULE_FILECHECKASSISTANT){
    INCLUDEPATH += \
        $$PWD/src

    FORMS += \
        $$PWD/src/SAKToolFileCheckAssistant.ui

    HEADERS += \
        $$PWD/src/QtCryptographicHashCalculator.h \
        $$PWD/src/SAKToolFileCheckAssistant.h

    SOURCES += \
        $$PWD/src/QtCryptographicHashCalculator.cc \
        $$PWD/src/SAKToolFileCheckAssistant.cc
}else {
    message("The Qt edition does not support the QCryptographicHash::Algorithm enum type, the program will not has file chcker module.")
}
