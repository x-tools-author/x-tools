/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QSpinBox>
#include <QComboBox>
#include <QModbusDevice>
#include <QModbusDataUnit>
#include <QStandardItemModel>
#include <QRegularExpressionValidator>

#include "SAKModbusCommonClientSection.hh"
#include "ui_SAKModbusCommonClientSection.h"

SAKModbusCommonClientSection::SAKModbusCommonClientSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonClientSection)
{
    ui->setupUi(this);
}

SAKModbusCommonClientSection::~SAKModbusCommonClientSection()
{
    delete ui;
}

SAKModbusCommonClientSection::ParametersContext
SAKModbusCommonClientSection::parametersContext()
{
    ParametersContext parasCtx;
    parasCtx.numberOfRetries = ui->retriesSpinBox->value();
    parasCtx.timeout = ui->timeoutSpinBox->value();
    return parasCtx;
}

void SAKModbusCommonClientSection::setUiEnable(bool isUnconnected)
{
    ui->timeoutSpinBox->setEnabled(isUnconnected);
    ui->retriesSpinBox->setEnabled(isUnconnected);
}

void SAKModbusCommonClientSection::initModbusClientParameters(QModbusClient *client)
{
    if (client){
        auto clientParacCtx = parametersContext();
        client->setNumberOfRetries(clientParacCtx.numberOfRetries);
        client->setTimeout(clientParacCtx.timeout);
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Parameter can not be null!");
    }
}
