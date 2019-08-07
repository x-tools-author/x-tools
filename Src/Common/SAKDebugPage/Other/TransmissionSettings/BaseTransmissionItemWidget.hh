/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef BASETRANSMISSSIONITEMWIDGET_HH
#define BASETRANSMISSSIONITEMWIDGET_HH

#include <QWidget>
#include <QSerialPort>

#include "SAKDebugPage.hh"

class BaseTransmissionItemWidget:public QWidget
{
    Q_OBJECT
public:
    BaseTransmissionItemWidget(SAKDebugPage *debugPage, QWidget *parent);

    struct TransmissionContext {
        bool enableHandleReceivedData;        
    };

public:
    /// 转发数据(将收到的数据通过指定的设备进行转发)
    virtual void transmit(QByteArray data, SAKDebugPage::OutputParameters parameters);
signals:
    /// 读取数据后，发射该信号
    void bytesRead(QByteArray data);

protected:
    SAKDebugPage *_debugPage;
    struct TransmissionContext transmissionContext;

    /// 处理接受到的数据（回传接受到的数据）
    void setEnableHandleReceivedData(bool enable);
};

#endif
