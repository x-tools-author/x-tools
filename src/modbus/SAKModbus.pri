# Serialbus was introduced in Qt 5.8.
isEqual(QT_MAJOR_VERSION, 5){
    !lessThan(QT_MINOR_VERSION, 7){
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_SERIALBUS
        }
    }
} else {
    qtHaveModule(serialbus){
        QT  += serialbus
        DEFINES+=SAK_IMPORT_MODULE_SERIALBUS
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_SERIALBUS){
INCLUDEPATH += \
    $$PWD \
    $$PWD/client \
    $$PWD/common \
    $$PWD/server

FORMS += \
    $$PWD/SAKModbusDebugPage.ui \
    $$PWD/common/SAKModbusCommonClientSection.ui \
    $$PWD/common/SAKModbusCommonHostSection.ui \
    $$PWD/common/SAKModbusCommonRegister.ui \
    $$PWD/common/SAKModbusCommonRegisterViewController.ui \
    $$PWD/common/SAKModbusCommonReigsterView.ui \
    $$PWD/common/SAKModbusCommonSerialPortSection.ui \
    $$PWD/common/SAKModbusCommonServerSection.ui

HEADERS += \
    $$PWD/SAKModbusDebugger.hh \
    $$PWD/client/SAKModbusClientController.hh \
    $$PWD/client/SAKModbusClientControllerSerialPort.hh \
    $$PWD/client/SAKModbusClientControllerTcp.hh \
    $$PWD/common/SAKModbusCommonClientSection.hh \
    $$PWD/common/SAKModbusCommonController.hh \
    $$PWD/common/SAKModbusCommonFlowLayout.hh \
    $$PWD/common/SAKModbusCommonHostSection.hh \
    $$PWD/common/SAKModbusCommonInterface.hh \
    $$PWD/common/SAKModbusCommonRegister.hh \
    $$PWD/common/SAKModbusCommonRegisterView.hh \
    $$PWD/common/SAKModbusCommonRegisterViewController.hh \
    $$PWD/common/SAKModbusCommonSerialPortSection.hh \
    $$PWD/common/SAKModbusCommonServerSection.hh \
    $$PWD/server/SAKModbusServerController.hh \
    $$PWD/server/SAKModbusServerControllerSerialPort.hh \
    $$PWD/server/SAKModbusServerControllerTcp.hh

SOURCES += \
    $$PWD/SAKModbusDebugger.cc \
    $$PWD/client/SAKModbusClientController.cc \
    $$PWD/client/SAKModbusClientControllerSerialPort.cc \
    $$PWD/client/SAKModbusClientControllerTcp.cc \
    $$PWD/common/SAKModbusCommonClientSection.cc \
    $$PWD/common/SAKModbusCommonController.cc \
    $$PWD/common/SAKModbusCommonFlowLayout.cc \
    $$PWD/common/SAKModbusCommonHostSection.cc \
    $$PWD/common/SAKModbusCommonInterface.cc \
    $$PWD/common/SAKModbusCommonRegister.cc \
    $$PWD/common/SAKModbusCommonRegisterView.cc \
    $$PWD/common/SAKModbusCommonRegisterViewController.cc \
    $$PWD/common/SAKModbusCommonSerialPortSection.cc \
    $$PWD/common/SAKModbusCommonServerSection.cc \
    $$PWD/server/SAKModbusServerController.cc \
    $$PWD/server/SAKModbusServerControllerSerialPort.cc \
    $$PWD/server/SAKModbusServerControllerTcp.cc
}
