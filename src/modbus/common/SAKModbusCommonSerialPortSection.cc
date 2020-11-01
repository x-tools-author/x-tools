#include "SAKModbusCommonSerialPortSection.hh"
#include "ui_SAKModbusCommonSerialPortSection.h"

SAKModbusCommonSerialPortSection::SAKModbusCommonSerialPortSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonSerialPortSection)
{
    ui->setupUi(this);
}

SAKModbusCommonSerialPortSection::~SAKModbusCommonSerialPortSection()
{
    delete ui;
}
