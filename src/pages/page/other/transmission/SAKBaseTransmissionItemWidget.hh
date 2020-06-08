/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKBASETRANSMISSSIONITEMWIDGET_HH
#define SAKBASETRANSMISSSIONITEMWIDGET_HH

#include <QWidget>
#include <QIODevice>

class SAKDebugPage;
class SAKBaseTransmissionItemWidget:public QWidget
{
    Q_OBJECT
public:
    SAKBaseTransmissionItemWidget(SAKDebugPage *debugPage, QWidget *parent);

    /// 转发函数，子类必须重新实现
    virtual void write(QByteArray data);
signals:
    /// 读取数据后，发射该信号
    void bytesRead(QByteArray data);
    /// 输出消息
    void requestOutputMessage(QString info, bool isInfo = true);
protected:
    SAKDebugPage *debugPage;
    void outputMessage();
};

#endif
