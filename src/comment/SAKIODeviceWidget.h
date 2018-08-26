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
#include <QApplication>

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
    void setRefreshPBtText(QString text);
private:
    SAKIODevice *device;
    SAKIODeviceControler *controler;
    QHBoxLayout *customControlerLayout;
    QTimer  *cycleTimer;
    Ui::SAKIODeviceWidget *ui;
    ///----------------------------------------------------------------
    void initUI();
    QByteArray dataBytes();
    void Connect();
private slots:
    void setCycleTime(QString time);
    void readCycleTime();

    void setOutputMode();
    void readOutputMode();

    void setInputMode();
    void readInputMode();

    void openFile();
    void saveOutputData();
    void textFormatControl();

    void outputTimeInfoCheckBoxClicked(bool checked);
    void outputReceiveDataOnlyCheckBoxClicked(bool cheaked);

    void openOrClose();
    void open(){emit need2open();}
    void close(){emit need2close();}
    void refresh(){emit need2refresh();}

    void afterDeviceOpen();
    void afterDeviceClose();

    void cycleTimerTimeout();
    void checkedBoxCycleClicked(bool checked);

    void outputInfo(QString info, QString color = "black", bool prefix = true);
    void outputErrorString(QString str);
    void outputInformationString(QString str);

    void bytesRead(QByteArray data);
    void bytesWritten(qint64 bytes);
    void writeBytes();

    void resetSendDataCount();
    void resetReceiveDataCount();
signals:
    void need2writeBytes(QByteArray data);
    void need2open();
    void need2close();
    void need2refresh();
};

#endif  // SAKIODEVICEWIDGET_H
