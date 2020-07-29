/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERTRANSMISSIONITEM_HH
#define SAKOTHERTRANSMISSIONITEM_HH

#include <QWidget>
#include <QIODevice>

class SAKDebugPage;
/// @brief base transmission item
class SAKOtherTransmissionItem:public QWidget
{
    Q_OBJECT
public:
    SAKOtherTransmissionItem(SAKDebugPage *debugPage, QWidget *parent);
    virtual void write(QByteArray data);
signals:
    void bytesRead(QByteArray data);
    void requestOutputMessage(QString info, bool isInfo = true);
protected:
    SAKDebugPage *mDebugPage;
    void outputMessage();
};

#endif
