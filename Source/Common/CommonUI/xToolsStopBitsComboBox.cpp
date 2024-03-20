/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakstopbitscombobox.h"

#include <QSerialPort>

SAKStopBitsComboBox::SAKStopBitsComboBox(QWidget* parent)
    : SAKComboBox(parent)
{
    addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
    addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
    addItem("2", QSerialPort::TwoStop);
}
