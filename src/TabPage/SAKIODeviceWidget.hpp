/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifndef SAKIODEVICEWIDGET_HPP
#define SAKIODEVICEWIDGET_HPP

#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QHBoxLayout>
#include <QDebug>
#include <QApplication>
#include <QSize>
#include <QPushButton>
#include <QComboBox>
#include <QMetaEnum>

#include "SAKIODevice.h"
#include "SAKIODeviceControler.h"
#include "AutoResponseSettingPanel.h"
#include "RWParameter.h"

class SAKHighlighterSettingPanel;

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

    /**
     * @brief The TextDisplayModel 输出输入显示模式
     */
    enum TextDisplayModel {
        Bin,
        Oct,
        Dec,
        Hex,
        Ascii,
        Local8bit
    };
    Q_ENUM(TextDisplayModel)
private:
    SAKIODevice                 *device                     = nullptr;
    SAKIODeviceControler        *controler                  = nullptr;
    AutoResponseSettingPanel    *autoResponseSettingPanel   = nullptr;
    QTimer                      *cycleTimer                 = nullptr;
    QHBoxLayout                 *customControlerLayout      = nullptr;
    QTimer                      *delayTimer                 = nullptr;
    Ui::SAKIODeviceWidget       *ui                         = nullptr;
    QTimer                      *clearInfoTimer             = nullptr;
    SAKHighlighterSettingPanel  *highlighterSettingPanel    = nullptr;
    QPushButton                 *highlighterSettingButton   = nullptr;

    QByteArray                  dataTemp                    = "(null)";
    const QSize                 rxtxSize                    = QSize(18, 18);

    QString                     inputTextMode               = "Hex";
    QString                     outputTextMode              = "Hex";

    QComboBox*                  inputTextModelComboBox      = nullptr;
    QComboBox*                  outputTextModelComboBox     = nullptr;

    QMetaEnum                   textModel;

    QPushButton*                rwParameterSettingButton    = nullptr;
    RWParameter                 rwParameterSettingDialog;
    ///----------------------------------------------------------------
    void initUI();
    QByteArray dataBytes();
    void Connect();

    QString readSetting(QString &option);
    void writeSetting(QString &option, QString &value);

    /**
     * @brief refreshOutputData -- 更新输出数据
     * @param data              -- 需要输出显示的数据
     * @param isReceivedData    -- true表示数据是接受到的数据，否则是发送数据
     */
    void refreshOutputData(QByteArray &data, bool isReceivedData = true);
private slots:
    virtual void updateRxImage();
    virtual void updateTxImage();

    void cancleBytesDelay();
    void cancleCycle();

    void setDelayTime(QString time);
    void readDelayTime();

    void setCycleTime(QString time);
    void readCycleTime();

    void setOutputMode(QString mode);
    void readOutputMode();

    void setInputMode(QString mode);
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
    void clearInfo();

    void bytesRead(QByteArray data);
    void bytesWritten(QByteArray data);
    void writeBytes();

    void resetSendDataCount();
    void resetReceiveDataCount();

    void setAutoResponseFlag(bool enableAutoResponse);
    void readAutoResponseFlag();

    void handleReadBytes(QByteArray data);
signals:
    void need2writeBytes(QByteArray data);
    void need2open();
    void need2close();
    void need2refresh();
};

#endif  // SAKIODEVICEWIDGET_H
