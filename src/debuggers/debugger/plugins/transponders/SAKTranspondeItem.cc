/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#include "SAKTranspondeItem.hh"

SAKTranspondeItem::SAKTranspondeItem(QWidget *parent)
    :SAKBaseListWidgetItemWidget(parent)
{

}

SAKTranspondeItem::SAKTranspondeItem(quint64 id, QWidget *parent)
    :SAKBaseListWidgetItemWidget(id, parent)
{

}

void SAKTranspondeItem::onBytesRead(QByteArray bytes)
{
    Q_UNUSED(bytes);
}
