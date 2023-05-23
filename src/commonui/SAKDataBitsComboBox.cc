/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QSerialPort>
#include "SAKDataBitsComboBox.hh"

SAKDataBitsComboBox::SAKDataBitsComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
    addItem("8", QSerialPort::Data8);
    addItem("7", QSerialPort::Data7);
    addItem("6", QSerialPort::Data6);
    addItem("5", QSerialPort::Data5);
}
