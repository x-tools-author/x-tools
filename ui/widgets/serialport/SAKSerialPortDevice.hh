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
#ifndef SAKSERIALPORTDEVICET_HH
#define SAKSERIALPORTDEVICET_HH

#include <QThread>
#include <QSerialPort>

class SAKSerialPortDebugPage;
class SAKSerialPortDevice:public QThread
{
    Q_OBJECT
public:
    SAKSerialPortDevice(const QString name,
                           const qint32 baudRate,
                           const QSerialPort::DataBits dataBits,
                           const QSerialPort::StopBits stopBits,
                           const QSerialPort::Parity parity,
                           SAKSerialPortDebugPage *debugPage,
                           QObject *parent = Q_NULLPTR);
    ~SAKSerialPortDevice();
    void readBytes();
    void writeBytes(QByteArray data);
private:
    void run();    
private:
    const QString               _name;
    const qint32                _baudRate;
    const QSerialPort::DataBits _dataBits;
    const QSerialPort::StopBits _stopBits;
    const QSerialPort::Parity   _parity;
    QSerialPort                 *serialPort;
    SAKSerialPortDebugPage *debugPage;

signals:
    void bytesRead(QByteArray);
    void bytesWriten(QByteArray);

    void deviceStatuChanged(bool opened);
    void messageChanged(QString message, bool isInfo);
};

#endif
