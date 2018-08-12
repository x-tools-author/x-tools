/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef UDPCLIENTWIDGET_H
#define UDPCLIENTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QApplication>
#include <QDebug>
#include <QHostInfo>
#include <QNetworkInterface>

#include "outputwindow.h"
#include "inputwindow.h"
#include "senddatapanel.h"
#include "UdpClientAssistant.h"

namespace Ui {
class UdpClientWidget;
}

class UdpClientWidget : public QWidget
{
    Q_OBJECT
public:
    UdpClientWidget(QWidget *parent = Q_NULLPTR);
    ~UdpClientWidget();
    /// -------------------------------------------------------
private:
    /// 串口
    UdpClientAssistant *mpUdpClientAssistant;
    /// 串口状态
    bool mIsOpen = false;
    Ui::UdpClientWidget *ui;
    OutputWindow *mpOutputWindow;
    InputWindow *mpInputWindow;
    SendDataPanel *mpSendDataPanel;
    /// -------------------------------------------------------
    /// 关联各种信号与槽
    void Connect();
    /// 初始化ip地址
    void InitHostAddress();
private slots:
    /// 发送数据
    void Write();
    /// 套接字打开后的一些操作
    void AfterUdpSocketClose();
    /// 套接字打开成功后的一些操作
    void AfterUdpSocketOpen();
    /// 打开套接字
    void Open();
signals:
    /// 关闭套接字
    void Need2Close();
    /// 外部关联该信号来打开套接字
    void Need2Open(QString hostAddress, QString port);
    /// 需要发送数据
    void Need2Write(QByteArray data, QString hostAddress, QString port);
};
#endif
