/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKTCPSERVERDEVICE_HH
#define SAKTCPSERVERDEVICE_HH

#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

class SAKDebugPage;
class SAKTcpServerDevice:public QThread
{
    Q_OBJECT
public:
    SAKTcpServerDevice(QString serverHost, quint16 serverPort, SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    void readBytes();
    void writeBytes(QByteArray data, QString host, quint16 port);
private:
    void run();    
private:
    QString localHost;
    quint16 localPort;
    bool enableCustomLocalSetting;
    QString serverHost;
    quint16 serverPort;
    SAKDebugPage *debugPage;
    QTcpServer *tcpServer;
private:
    void afterDisconnected();
    void newConnection();

    QList<QTcpSocket*> clients;
signals:
    void bytesRead(QByteArray data, QString host, quint16 port);
    void bytesWritten(QByteArray data, QString host, quint16 port);

    void deviceStatuChanged(bool opened);
    void messageChanged(QString message, bool isInfo);
    void newClientConnected(QString host, quint16 port);
};

#endif
