/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#ifndef SAKTRANSPONDER_HH
#define SAKTRANSPONDER_HH

#include <QVariant>
#include "SAKBaseListWidgetItemWidget.hh"

class SAKDebuggerDevice;
class SAKTransponder : public SAKBaseListWidgetItemWidget
{
    Q_OBJECT
public:
    SAKTransponder(QWidget *parent = Q_NULLPTR);
    SAKTransponder(quint64 id, QWidget *parent = Q_NULLPTR);
    void setupDevice();
    virtual QVariant parametersContext() = 0;
protected:
    void onBytesRead(QByteArray bytes) final;
private:
    virtual SAKDebuggerDevice *device() = 0;
    virtual void onDeviceStateChanged(bool opened) = 0;
signals:
    void parametersContextChanged();
};

#endif // SAKTRANSPONDEITEM_HH
