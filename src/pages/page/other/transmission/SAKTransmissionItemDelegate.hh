/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTRANSMISSIONITEMDELEGATE_HH
#define SAKTRANSMISSIONITEMDELEGATE_HH

#include <QListWidget>
#include <QAbstractItemDelegate>

class SAKDebugPage;

class SAKTransmissionItemDelegate:public QAbstractItemDelegate
{
    Q_OBJECT
public:
    SAKTransmissionItemDelegate(SAKDebugPage *page, QObject *parent = Q_NULLPTR);

    enum TransmissionItemType {
        SerialPortType,
        UdpType,
        TcpType
    };
private:
    SAKDebugPage *debugPage;
    TransmissionItemType transmissionItemType = SerialPortType;
};

#endif
