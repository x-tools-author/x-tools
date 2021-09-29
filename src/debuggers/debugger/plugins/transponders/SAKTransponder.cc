/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#include "SAKDebuggerDevice.hh"
#include "SAKTransponderItem.hh"

SAKTransponderItem::SAKTransponderItem(QWidget *parent)
    :SAKBaseListWidgetItemWidget(parent)
{

}

SAKTransponderItem::SAKTransponderItem(quint64 id, QWidget *parent)
    :SAKBaseListWidgetItemWidget(id, parent)
{

}

void SAKTransponderItem::setupDevice()
{
    SAKDebuggerDevice *dev = device();
    if (dev) {
        dev->setParametersContext(parametersContext());
        connect(this, &SAKTransponderItem::parametersContextChanged,
                this, [=](){
            dev->setParametersContext(parametersContext());
        });
        connect(dev, &SAKDebuggerDevice::bytesRead,
                this, &SAKTransponderItem::invokeWriteCookedBytes);
        connect(dev, &SAKDebuggerDevice::errorOccurred,
                this, [=](){
            onDeviceStateChanged(false);
        });
        connect(dev, &SAKDebuggerDevice::finished,
                this, [=](){
            onDeviceStateChanged(false);
        });
        connect(dev, &SAKDebuggerDevice::started,
                this, [=](){
            onDeviceStateChanged(true);
        });
    }
}

void SAKTransponderItem::onBytesRead(QByteArray bytes)
{
    if (enable()) {
        SAKDebuggerDevice *dev = device();
        if (dev) {
            dev->writeBytes(bytes);
        }
    }
}
