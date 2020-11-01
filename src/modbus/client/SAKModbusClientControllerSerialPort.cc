#include "SAKModbusCommonClientSection.hh"
#include "SAKModbusCommonSerialPortSection.hh"
#include "SAKModbusClientControllerSerialPort.hh"

SAKModbusClientControllerSerialPort::SAKModbusClientControllerSerialPort(QWidget *parent)
    :SAKModbusClientController(parent)
{
    mSerialPortSection = new SAKModbusCommonSerialPortSection(this);
    mClientSection = new SAKModbusCommonClientSection(this);
    appendSection(mSerialPortSection);
    appendSection(mClientSection);
}
