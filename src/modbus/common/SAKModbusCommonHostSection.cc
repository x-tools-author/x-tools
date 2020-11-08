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

#include "SAKModbusCommonHostSection.hh"
#include "ui_SAKModbusCommonHostSection.h"

SAKModbusCommonHostSection::SAKModbusCommonHostSection(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKModbusCommonHostSection)
{
    ui->setupUi(this);

    QString localHost("127.0.0.1");
    auto comboBox = ui->hostComboBox;
    comboBox->clear();
    comboBox->addItem(QString("::"));
    comboBox->addItem(QString("::1"));
    comboBox->addItem(QString("0.0.0.0"));
    comboBox->addItem(localHost);
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    for(auto var:addresses){
        if (var.protocol() == QAbstractSocket::IPv4Protocol) {
            if (var.toString().compare(localHost) == 0){
                continue;
            }
            comboBox->addItem(var.toString());
        }
    }
    comboBox->setCurrentText(localHost);
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
