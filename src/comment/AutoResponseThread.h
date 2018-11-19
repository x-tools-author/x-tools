/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef AUTORESPONSETHREAD_H
#define AUTORESPONSETHREAD_H

#include <QThread>

class AutoResponseThread:public QThread
{
    Q_OBJECT
public:
    AutoResponseThread(QObject *parent = Q_NULLPTR);
    ~AutoResponseThread();
protected:
    void run();
private slots:
    void handleReadBytes(QByteArray bytes);
signals:
    void autoResponse(QByteArray bytes);
};


#endif
