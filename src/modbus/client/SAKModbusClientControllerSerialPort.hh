#ifndef SAKMODBUSCLIENTCONTROLLERSERIALPORT_HH
#define SAKMODBUSCLIENTCONTROLLERSERIALPORT_HH

#include "SAKModbusClientController.hh"

class SAKModbusCommonClientSection;
class SAKModbusCommonSerialPortSection;
class SAKModbusClientControllerSerialPort : public SAKModbusClientController
{
public:
    SAKModbusClientControllerSerialPort(QWidget *parent = Q_NULLPTR);
private:
    SAKModbusCommonSerialPortSection *mSerialPortSection;
    SAKModbusCommonClientSection *mClientSection;
};

#endif // SAKMODBUSCLIENTCONTROLLERSERIALPORT_HH
