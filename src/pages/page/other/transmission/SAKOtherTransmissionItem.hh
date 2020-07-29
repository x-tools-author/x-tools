/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
