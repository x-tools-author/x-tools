/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef TCPSERVERSAKIODEVICECONTROLER_H
#define TCPSERVERSAKIODEVICECONTROLER_H

#include "SAKIODeviceControler.h"
#include <QTcpSocket>

namespace Ui {
class TcpServerSAKIODeviceControler;
}

class TcpServerSAKIODeviceControler:public SAKIODeviceControler
{
    Q_OBJECT
public:
    TcpServerSAKIODeviceControler(QWidget *parent = Q_NULLPTR);
    ~TcpServerSAKIODeviceControler();

    virtual void afterDeviceOpen();
    virtual void afterDeviceClose();
public slots:
    virtual void open();
    virtual void refresh();
private:
    Ui::TcpServerSAKIODeviceControler *ui = nullptr;
    /// ---------------------------------------
    void initUi();
    void readServerPort();
private slots:
    void changedClients(QList <QTcpSocket *>clients);
    void setCurrentClient(QString text);
    void setServerPort(QString port);
signals:
    void need2changeCurrentClients(QString address, QString port);
};

#endif
