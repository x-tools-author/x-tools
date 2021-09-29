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
#include "SAKTransponder.hh"

SAKTransponder::SAKTransponder(QWidget *parent)
    :SAKBaseListWidgetItemWidget(parent)
{

}

SAKTransponder::SAKTransponder(quint64 id, QWidget *parent)
    :SAKBaseListWidgetItemWidget(id, parent)
{

}

void SAKTransponder::setupDevice()
{
    SAKDebuggerDevice *dev = device();
    if (dev) {
        dev->setParametersContext(parametersContext());
        connect(this, &SAKTransponder::parametersContextChanged,
                this, [=](){
            dev->setParametersContext(parametersContext());
        });
        connect(dev, &SAKDebuggerDevice::bytesRead,
                this, &SAKTransponder::invokeWriteCookedBytes);
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

void SAKTransponder::onBytesRead(QByteArray bytes)
{
    if (enable()) {
        SAKDebuggerDevice *dev = device();
        if (dev) {
            dev->writeBytes(bytes);
        }
    }
}
