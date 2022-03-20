/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QHostAddress>
#include <QNetworkInterface>

#include "SAKCommonInterface.hh"
#include "SAKModbusCommonHostSection.hh"
#include "ui_SAKModbusCommonHostSection.h"

SAKModbusCommonHostSection::SAKModbusCommonHostSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonHostSection)
{
    ui->setupUi(this);
    SAKCommonInterface::addIpItemsToComboBox(ui->hostComboBox);
}

SAKModbusCommonHostSection::~SAKModbusCommonHostSection()
{
    delete ui;
}

SAKModbusCommonHostSection::ParametersContext SAKModbusCommonHostSection::parametersContext()
{
    ParametersContext parasCtx;
    parasCtx.host = ui->hostComboBox->currentText();
    parasCtx.port = ui->portLineSpinBox->value();
    return parasCtx;
}

void SAKModbusCommonHostSection::initModbusDeviceParamerers(QModbusDevice *dev)
{
    if (dev){
        auto deviceParasCtx = parametersContext();
        dev->setConnectionParameter(QModbusDevice::NetworkPortParameter, deviceParasCtx.port);
        dev->setConnectionParameter(QModbusDevice::NetworkAddressParameter, deviceParasCtx.host);
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Parameter can not be null!");
    }
}
