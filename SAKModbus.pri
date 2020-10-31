qtHaveModule(serialbus){
    QT  += serialbus
    DEFINES+=SAK_IMPORT_MODULE_SERIALBUS
}

contains(DEFINES, SAK_IMPORT_CHARTS_MODULE){
INCLUDEPATH += \
    $$PWD/src/modbus

HEADERS += \
    $$PWD/src/modbus/SAKModbusDebugPage.hh

SOURCES += \
    $$PWD/src/modbus/SAKModbusDebugPage.cc
}

FORMS += \
    $$PWD/src/modbus/SAKModbusDebugPage.ui \
    $$PWD/src/modbus/common/SAKModbusCommonClientParametersController.ui \
    $$PWD/src/modbus/common/SAKModbusCommonComController.ui \
    $$PWD/src/modbus/common/SAKModbusCommonHostController.ui \
    $$PWD/src/modbus/common/SAKModbusCommonServerParametersController.ui

HEADERS += \
    $$PWD/src/modbus/client/SAKModbusClientController.hh \
    $$PWD/src/modbus/client/SAKModbusControllerSerialportClient.hh \
    $$PWD/src/modbus/client/SAKModbusControllerTcpClient.hh \
    $$PWD/src/modbus/common/SAKModbusCommonController.hh \
    $$PWD/src/modbus/common/SAKModuleFlowLayout.hh

SOURCES += \
    $$PWD/src/modbus/client/SAKModbusClientController.cc \
    $$PWD/src/modbus/client/SAKModbusControllerSerialportClient.cc \
    $$PWD/src/modbus/client/SAKModbusControllerTcpClient.cc \
    $$PWD/src/modbus/client/sakmodbuscontrollertcpclient.cc \
    $$PWD/src/modbus/common/SAKModbusCommonController.cc \
    $$PWD/src/modbus/common/SAKModuleFlowLayout.cc
