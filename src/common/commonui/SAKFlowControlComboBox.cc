/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QSerialPort>
#include "SAKFlowControlComboBox.h"

SAKFlowControlComboBox::SAKFlowControlComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
    addItem(tr("No"), QSerialPort::NoFlowControl);
    addItem(tr("Hardware"), QSerialPort::HardwareControl);
    addItem(tr("Software"), QSerialPort::SoftwareControl);
}
