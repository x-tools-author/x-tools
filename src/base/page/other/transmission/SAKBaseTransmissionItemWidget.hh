/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
