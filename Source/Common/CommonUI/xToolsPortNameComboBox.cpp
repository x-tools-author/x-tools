/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsPortNameComboBox.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStandardItemModel>

xToolsPortNameComboBox::xToolsPortNameComboBox(QWidget* parent)
    : xToolsComboBox(parent)
{
    refresh();
}

void xToolsPortNameComboBox::refresh()
{
    clear();
    QList<QSerialPortInfo> coms = QSerialPortInfo::availablePorts();
    QStandardItemModel* itemModel = new QStandardItemModel(this);
    for (auto& var : coms) {
        QStandardItem* item = new QStandardItem(var.portName());
        item->setToolTip(var.description());
        itemModel->appendRow(item);
    }

    setModel(itemModel);
}
