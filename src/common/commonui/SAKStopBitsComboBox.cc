/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include <QSerialPort>
#include "SAKStopBitsComboBox.h"

SAKStopBitsComboBox::SAKStopBitsComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
    addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    addItem("2", QSerialPort::TwoStop);
}
