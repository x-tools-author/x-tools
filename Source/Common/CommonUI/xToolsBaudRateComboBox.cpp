/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <QSerialPort>
#include <QSerialPortInfo>

#include "xToolsBaudRateComboBox.h"

xToolsBaudRateComboBox::xToolsBaudRateComboBox(QWidget *parent)
    : xToolsComboBox(parent)
{
    clear();
    QList<qint32> bd = QSerialPortInfo::standardBaudRates();
    for (auto &var : bd) {
        addItem(QString::number(var), QVariant::fromValue(var));
    }

    setCurrentText("9600");
}
