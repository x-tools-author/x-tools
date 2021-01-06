# Serialbus was introduced in Qt 5.8.
isEqual(QT_MAJOR_VERSION, 5){
    !lessThan(QT_MINOR_VERSION, 7){
        qtHaveModule(serialbus){
            QT  += serialbus
            DEFINES+=SAK_IMPORT_MODULE_SERIALBUS
        }
    }
}

contains(DEFINES, SAK_IMPORT_MODULE_SERIALBUS){
INCLUDEPATH += \
    $$PWD/src/modbus \
    $$PWD/src/modbus/client \
    $$PWD/src/modbus/common \
    $$PWD/src/modbus/server \

FORMS += \
    $$PWD/src/modbus/SAKModbusDebugPage.ui \
    $$PWD/src/modbus/common/SAKModbusCommonClientSection.ui \
    $$PWD/src/modbus/common/SAKModbusCommonHostSection.ui \
    $$PWD/src/modbus/common/SAKModbusCommonRegister.ui \
    $$PWD/src/modbus/common/SAKModbusCommonRegisterViewController.ui \
    $$PWD/src/modbus/common/SAKModbusCommonReigsterView.ui \
    $$PWD/src/modbus/common/SAKModbusCommonSerialPortSection.ui \
    $$PWD/src/modbus/common/SAKModbusCommonServerSection.ui

HEADERS += \
    $$PWD/src/modbus/SAKModbusDebugPage.hh \
    $$PWD/src/modbus/client/SAKModbusClientController.hh \
    $$PWD/src/modbus/client/SAKModbusClientControllerSerialPort.hh \
    $$PWD/src/modbus/client/SAKModbusClientControllerTcp.hh \
    $$PWD/src/modbus/common/SAKModbusCommonClientSection.hh \
    $$PWD/src/modbus/common/SAKModbusCommonController.hh \
    $$PWD/src/modbus/common/SAKModbusCommonFlowLayout.hh \
    $$PWD/src/modbus/common/SAKModbusCommonHostSection.hh \
    $$PWD/src/modbus/common/SAKModbusCommonInterface.hh \
    $$PWD/src/modbus/common/SAKModbusCommonRegister.hh \
    $$PWD/src/modbus/common/SAKModbusCommonRegisterView.hh \
    $$PWD/src/modbus/common/SAKModbusCommonRegisterViewController.hh \
    $$PWD/src/modbus/common/SAKModbusCommonSerialPortSection.hh \
    $$PWD/src/modbus/common/SAKModbusCommonServerSection.hh \
    $$PWD/src/modbus/server/SAKModbusServerController.hh \
    $$PWD/src/modbus/server/SAKModbusServerControllerSerialPort.hh \
    $$PWD/src/modbus/server/SAKModbusServerControllerTcp.hh

SOURCES += \
    $$PWD/src/modbus/SAKModbusDebugPage.cc \
    $$PWD/src/modbus/client/SAKModbusClientController.cc \
    $$PWD/src/modbus/client/SAKModbusClientControllerSerialPort.cc \
    $$PWD/src/modbus/client/SAKModbusClientControllerTcp.cc \
    $$PWD/src/modbus/common/SAKModbusCommonClientSection.cc \
    $$PWD/src/modbus/common/SAKModbusCommonController.cc \
    $$PWD/src/modbus/common/SAKModbusCommonFlowLayout.cc \
    $$PWD/src/modbus/common/SAKModbusCommonHostSection.cc \
    $$PWD/src/modbus/common/SAKModbusCommonInterface.cc \
    $$PWD/src/modbus/common/SAKModbusCommonRegister.cc \
    $$PWD/src/modbus/common/SAKModbusCommonRegisterView.cc \
    $$PWD/src/modbus/common/SAKModbusCommonRegisterViewController.cc \
    $$PWD/src/modbus/common/SAKModbusCommonSerialPortSection.cc \
    $$PWD/src/modbus/common/SAKModbusCommonServerSection.cc \
    $$PWD/src/modbus/server/SAKModbusServerController.cc \
    $$PWD/src/modbus/server/SAKModbusServerControllerSerialPort.cc \
    $$PWD/src/modbus/server/SAKModbusServerControllerTcp.cc
}
