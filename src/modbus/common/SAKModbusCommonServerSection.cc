/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKModbusCommonServerSection.hh"
#include "ui_SAKModbusCommonServerSection.h"

SAKModbusCommonServerSection::SAKModbusCommonServerSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonServerSection)
{
    ui->setupUi(this);
}

SAKModbusCommonServerSection::~SAKModbusCommonServerSection()
{

}

void SAKModbusCommonServerSection::initModbusServerParameters(QModbusServer *server)
{
    if (server){
        auto serverAddress = ui->serverAddressSpinBox->value();
        auto listenOnly = ui->listenLonlyCheckBox->isChecked();
        auto isBusy = ui->busyCheckBox->isChecked();
        server->setServerAddress(serverAddress);
        server->setValue(QModbusServer::ListenOnlyMode, listenOnly);
        server->setValue(QModbusServer::DeviceBusy, isBusy ? 0xffff : 0x0000);
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Parameter can not be null!");
    }
}
