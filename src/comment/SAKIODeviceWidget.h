/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKIODEVICEWIDGET_H
#define SAKIODEVICEWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QHBoxLayout>
#include <QDebug>

#include "SAKIODevice.h"
#include "SAKIODeviceControler.h"

namespace Ui{
class SAKIODeviceWidget;
}

class SAKIODeviceWidget : public QWidget
{
    Q_OBJECT
public:
    SAKIODeviceWidget(SAKIODevice *_device, SAKIODeviceControler *_controler, QWidget *parent = Q_NULLPTR);
    ~SAKIODeviceWidget();

    void setCustomControler(SAKIODeviceControler *controler);
    void setState(bool _deviceIsOpen){deviceIsOpen = _deviceIsOpen;}
    /// 设备状态：打开或者关闭（打开状态标识设备就绪，否则标识设备未可用！）
    bool state(){return deviceIsOpen;}
private:
    SAKIODevice *device;
    SAKIODeviceControler *controler;
    QHBoxLayout *customControlerLayout;
    bool deviceIsOpen;
    QTimer  *cycleTimer;
    Ui::SAKIODeviceWidget *ui;
    ///----------------------------------------------------------------
    QByteArray dataBytes();
    void Connect();
private slots:
    void openOrClose();

    void open(){emit need2open();}
    void close(){emit need2close();}
    void refresh(){emit need2refresh();}

    void afterDeviceOpen();
    void afterDeviceClose();

    void cycleTimerTimeout();
    void CheckedBoxCycleClicked(bool checked);

    void outputInfo(QString info, QString color = "black", bool prefix = false);

    void bytesRead(QByteArray data);
    void writeBytes();
signals:
    void need2writeBytes(QByteArray data);
    void need2open();
    void need2close();
    void need2refresh();
};

#endif  // SAKIODEVICEWIDGET_H
