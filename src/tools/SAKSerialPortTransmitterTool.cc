/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKSerialPortTransmitterTool.hh"

SAKSerialPortTransmitterTool::SAKSerialPortTransmitterTool(QObject *parent)
    : SAKTransmitterTool{"SAK.SerialPortTransmitterTool", parent}
{

}

QVariant SAKSerialPortTransmitterTool::itemContext(int index)
{
    return QVariant();
}

int SAKSerialPortTransmitterTool::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int SAKSerialPortTransmitterTool::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant SAKSerialPortTransmitterTool::data(const QModelIndex &index,
                                            int role) const
{
    return 0;
}

bool SAKSerialPortTransmitterTool::setData(const QModelIndex &index,
                                           const QVariant &value,
                                           int role)
{
    return 0;
}

bool SAKSerialPortTransmitterTool::insertRows(int row,
                                              int count,
                                              const QModelIndex &parent)
{
    return 0;
}

bool SAKSerialPortTransmitterTool::removeRows(int row,
                                              int count,
                                              const QModelIndex &parent)
{
    return 0;
}

QVariant SAKSerialPortTransmitterTool::headerData(int section,
                                                  Qt::Orientation orientation,
                                                  int role) const
{
    return 0;
}
