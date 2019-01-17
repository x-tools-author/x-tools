/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef UDPSAKIODEVICECONTROL_H
#define UDPSAKIODEVICECONTROL_H

#include "SAKIODeviceControler.h"

#include <QLineEdit>
#include <QComboBox>

#define LOCAL_PORT      QString("LocalPort")
#define PEER_PORT       QString("PeerPort")
#define PEER_ADDRESS    QString("PeerAddress")

namespace Ui {
class UdpSAKIODeviceControler;
}

class UdpSAKIODeviceControler : public SAKIODeviceControler
{
    Q_OBJECT
public:
    UdpSAKIODeviceControler(QWidget *parent = Q_NULLPTR);
    ~UdpSAKIODeviceControler();

    virtual void afterDeviceOpen();
    virtual void afterDeviceClose();
public slots:
    virtual void open();
    virtual void refresh();
private:
    QComboBox* localHostAddress = nullptr;
    QLineEdit* localPort = nullptr;
    QLineEdit* peerPort  = nullptr;
    QLineEdit* peerAddress = nullptr;

    Ui::UdpSAKIODeviceControler *ui = nullptr;
    ///---------------------------------------------------------
    void initUi();

    void readLocalePort();
    void readPeerPort();
    void readPeerAddress();
private slots:
    void setLocalePort(QString port);
    void setPeerPort(QString port);
    void setPeerAddress(QString address);
};

#endif
