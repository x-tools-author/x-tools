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
#ifndef SAKUDPDEVICE_HH
#define SAKUDPDEVICE_HH

#include <QMutex>
#include <QThread>
#include <QUdpSocket>

class SAKDebugPage;
class SAKUdpDevice:public QThread
{
    Q_OBJECT
public:
    SAKUdpDevice(QString localHost, quint16 localPort,
                 bool enableCustomLocalSetting,
                 QString targetHost, quint16 targetPort,
                 SAKDebugPage *debugPage,
                 QObject *parent = Q_NULLPTR);
    ~SAKUdpDevice();

    /**
     * @brief readBytes 读取数据
     */
    void readBytes();

    /**
     * @brief writeBytes 写数据
     * @param data 代写数据
     */
    void writeBytes(QByteArray data);

    /**
     * @brief 参数上下文
     */
    struct ParametersContext{
        bool enableUnicast;
        bool enableMulticast;
        bool enableBroadcast;
        quint16 broadcastPort;

        struct MulticastInfo{
            QString address;
            quint16 port;
        };
        QList<MulticastInfo> multicastInfoList;
    };

    /**
     * @brief setUnicastEnable 启用/禁止单播功能
     * @param enable 该值为true时，启用单播功能，否则禁止单播功能
     */
    void setUnicastEnable(bool enable);

    /**
     * @brief setBroadcastInfo 启用/禁止广播功能
     * @param enable 该值为true时，启用广播功能，否则禁止广播功能
     */
    void setBroadcastEnable(bool enable);

    /**
     * @brief setBroadcastPort 设置广播端口
     * @param port 广播端口
     */
    void setBroadcastPort(quint16 port);

    /**
     * @brief addMulticastInfo 添加组播
     * @param address 组播地址
     * @param port 组播端口
     */
    void addMulticastInfo(QString address, quint16 port);

    /**
     * @brief removeMulticastInfo 移除组播
     * @param address 组播地址
     * @param port 组播端口
     */
    void removeMulticastInfo(QString address, quint16 port);

    /**
     * @brief setMulticastEnable 启用/静止组播功能（总开关）
     * @param enable 该值为true是，使能组播功能，否则禁止全部组播。
     */
    void setMulticastEnable(bool enable);
private:
    QMutex parametersContextMutex;
    ParametersContext parametersContext;
    const ParametersContext parametersContextInstance();

private:
    void run();    
private:
    QString localHost;
    quint16 localPort;
    bool enableCustomLocalSetting;
    QString targetHost;
    quint16 targetPort;
    SAKDebugPage *debugPage;
    QUdpSocket *udpSocket;

signals:
    void bytesRead(QByteArray);
    void bytesWriten(QByteArray);

    void deviceStatuChanged(bool opened);
    void messageChanged(QString message, bool isInfo);
};
Q_DECLARE_METATYPE(SAKUdpDevice::ParametersContext::MulticastInfo)
#endif
