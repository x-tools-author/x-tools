/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTDEVICE_HH
#define SAKUDPCLIENTDEVICE_HH

#include <QMutex>
#include <QThread>
#include <QUdpSocket>

#include "SAKDebugPageDevice.hh"

class SAKUdpClientDebugPage;
class SAKUdpClientDevice:public SAKDebugPageDevice
{
    Q_OBJECT
public:
    SAKUdpClientDevice(SAKUdpClientDebugPage *debugPage, QObject *parent = Q_NULLPTR);
    ~SAKUdpClientDevice();

    /**
     * @brief 参数上下文
     */
    struct ParametersContext{
        bool enableUnicast;     // 允许单播
        bool enableMulticast;   // 允许组播
        bool enableBroadcast;   // 允许广播
        quint16 broadcastPort;  // 广播端口

        struct MulticastInfo{   // 组播信息
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
protected:
    void run() final;
private:
    QMutex parametersContextMutex;
    ParametersContext parametersContext;    
    QString localHost;
    quint16 localPort;
    bool enableCustomLocalSetting;
    QString targetHost;
    quint16 targetPort;
    SAKUdpClientDebugPage *debugPage;
    QUdpSocket *udpSocket;
private:
    const ParametersContext parametersContextInstance();
};
Q_DECLARE_METATYPE(SAKUdpClientDevice::ParametersContext::MulticastInfo)
#endif
