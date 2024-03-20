/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsDataBitsComboBox.h"

#include <QSerialPort>

xToolsDataBitsComboBox::xToolsDataBitsComboBox(QWidget* parent)
    : xToolsComboBox(parent)
{
    addItem("8", QSerialPort::Data8);
    addItem("7", QSerialPort::Data7);
    addItem("6", QSerialPort::Data6);
    addItem("5", QSerialPort::Data5);
}
