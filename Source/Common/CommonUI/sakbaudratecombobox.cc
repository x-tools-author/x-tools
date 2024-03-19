/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QSerialPort>
#include <QSerialPortInfo>

#include "sakbaudratecombobox.h"

SAKBaudRateComboBox::SAKBaudRateComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
    clear();
    QList<qint32> bd = QSerialPortInfo::standardBaudRates();
    for (auto &var : bd) {
        addItem(QString::number(var), QVariant::fromValue(var));
    }

    setCurrentText("9600");
}
