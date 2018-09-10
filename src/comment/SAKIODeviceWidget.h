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
#include <QSize>

#include "SAKIODevice.h"
#include "SAKIODeviceControler.h"

namespace Ui {
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
    SAKIODevice *device =                   NULL;
    SAKIODeviceControler *controler =       NULL;
    QHBoxLayout *customControlerLayout =    NULL;
    QTimer  *cycleTimer =                   NULL;
    QTimer  *delayTimer =                   NULL;
    Ui::SAKIODeviceWidget *ui =             NULL;

    QByteArray dataTemp = "(null)";
    const QSize rxtxSize = QSize(18, 18);

    typedef enum __TextMode{
        TextModeBin,
        TextModeOct,
        TextModeDec,
        TextModeHex,
        TextModeAscii,
        TextModeUtf8
    }TextMode;

    TextMode outputTextMode = TextModeAscii;
    TextMode inputTextMode  = TextModeAscii;
    ///----------------------------------------------------------------
    void initUI();
    QByteArray dataBytes();
    void Connect();
private slots:
    void cancleBytesDelay();
    void cancleCycle();

    void setDelayTime(QString time);
    void readDelayTime();

    void setCycleTime(QString time);
    void readCycleTime();

    void setOutputMode(int mode);
    void readOutputMode();

    void setInputMode(int mode);
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
    void delayTimerTimeout();
    void checkedBoxCycleClicked(bool checked);
    void checkedBoxDelayClicked(bool checked);

    void outputInfo(QString info, QString color = "black", bool prefix = true);
    void outputErrorString(QString str);
    void outputInformationString(QString str);

    void bytesRead(QByteArray data);
    void bytesWritten(qint64 bytes);
    void writeBytes();

    void resetSendDataCount();
    void resetReceiveDataCount();

    virtual void updateRxImage();
    virtual void updateTxImage();
signals:
    void need2writeBytes(QByteArray data);
    void need2open();
    void need2close();
    void need2refresh();
};

#endif  // SAKIODEVICEWIDGET_H
