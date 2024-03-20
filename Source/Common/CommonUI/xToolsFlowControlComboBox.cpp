/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsFlowControlComboBox.h"

#include <QSerialPort>

xToolsFlowControlComboBox::xToolsFlowControlComboBox(QWidget* parent)
    : xToolsComboBox(parent)
{
    addItem(tr("No"), QSerialPort::NoFlowControl);
    addItem(tr("Hardware"), QSerialPort::HardwareControl);
    addItem(tr("Software"), QSerialPort::SoftwareControl);
}
