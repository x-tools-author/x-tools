/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SERIALPORTWIDGET_H
#define SERIALPORTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QApplication>
#include <QDebug>

#include "outputwindow.h"
#include "inputwindow.h"
#include "senddatapanel.h"
#include "SerialPortAssistant.h"

namespace Ui {
class SerialportWidget;
}

class SerialPortWidget : public QWidget
{
    Q_OBJECT
public:
    SerialPortWidget(QWidget *parent = Q_NULLPTR);
    ~SerialPortWidget();
    /// -------------------------------------------------------
private:
    /// 串口
    SerialportAssistant *mpSerialportAssistant;
    /// 串口状态
    bool mIsOpen = false;
    Ui::SerialportWidget *ui;
    OutputWindow *mpOutputWindow;
    InputWindow *mpInputWindow;
    SendDataPanel *mpSendDataPanel;
    /// -------------------------------------------------------
    /// 关联各种信号与槽
    void Connect();
    /// 标准波特率
    void SetBaudRate();
private slots:
    /// 发送数据
    void Write();
    /// 串口关闭后进行的一些操作
    void AfterSerialportClose();
    /// 串口打开成功后进行的一些操作
    void AfterSerialportOpen();
    /// 打开串口
    void Open();
    /// 获取串口设备
    void RefreshDevice();
signals:
    /// 关闭串口
    void Need2Close();
    /// 外部关联该信号来打开串口
    void Need2Open(QString portName, QString baudRate, QString dataBits, QString stopBits, QString parity);
    /// 需要发送数据
    void Need2Write(QByteArray data);
};
#endif
