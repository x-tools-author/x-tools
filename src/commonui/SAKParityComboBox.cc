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
#include "SAKParityComboBox.hh"

SAKParityComboBox::SAKParityComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
    addItem(tr("No"), QSerialPort::NoParity);
    addItem(tr("Even"), QSerialPort::EvenParity);
    addItem(tr("Odd"), QSerialPort::OddParity);
    addItem(tr("Space"), QSerialPort::SpaceParity);
    addItem(tr("Mark"), QSerialPort::MarkParity);
}
