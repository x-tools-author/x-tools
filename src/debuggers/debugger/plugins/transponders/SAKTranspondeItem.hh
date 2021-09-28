/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#ifndef SAKTRANSPONDEITEM_HH
#define SAKTRANSPONDEITEM_HH

#include "SAKBaseListWidgetItemWidget.hh"

class SAKDebuggerDevice;
class SAKTranspondeItem : public SAKBaseListWidgetItemWidget
{
    Q_OBJECT
public:
    SAKTranspondeItem(QWidget *parent = Q_NULLPTR);
    SAKTranspondeItem(quint64 id, QWidget *parent = Q_NULLPTR);
protected:
    void onBytesRead(QByteArray bytes) final;
};

#endif // SAKTRANSPONDEITEM_HH
