/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKTransmitterTool.hh"

SAKTransmitterTool::SAKTransmitterTool(const char *logCategory, QObject *parent)
    : SAKTableModelTool{logCategory, parent}
{

}

int SAKTransmitterTool::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int ret = mToolVector.length();
    return ret;
}

bool SAKTransmitterTool::removeRows(int row, int count,
                                    const QModelIndex &parent)
{
    Q_UNUSED(parent)
    mToolVector.remove(row, count);
    return true;
}
