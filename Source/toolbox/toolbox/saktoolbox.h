/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTOOLBOX_H
#define SAKTOOLBOX_H

#include <QObject>
#include <QVariant>

#include "sakbasetool.h"
#include "sakcommunicationtool.h"
#include "sakemittertool.h"
#include "sakprestorertool.h"
#include "sakresponsertool.h"
#include "sakserialporttransmittertool.h"
#include "sakstatisticiantool.h"
#include "sakstorertool.h"
#include "saktcptransmittertool.h"
#include "sakudptransmittertool.h"
#include "sakvelometertool.h"
#include "sakwebsockettransmittertool.h"

class SAKToolBox : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant communicator READ communicator NOTIFY communicatorChanged)
    Q_PROPERTY(QVariant emitter READ emitter CONSTANT)
    Q_PROPERTY(QVariant responser READ responser CONSTANT)
    Q_PROPERTY(QVariant storer READ storer CONSTANT)
    Q_PROPERTY(QVariant prestorer READ prestorer CONSTANT)
    Q_PROPERTY(QVariant rxVelometer READ rxVelometer CONSTANT)
    Q_PROPERTY(QVariant txVelometer READ txVelometer CONSTANT)
    Q_PROPERTY(QVariant rxStatistician READ rxStatistician CONSTANT)
    Q_PROPERTY(QVariant txStatistician READ txStatistician CONSTANT)

    Q_PROPERTY(QVariant udpTransmitter READ udpTransmitter CONSTANT)
    Q_PROPERTY(QVariant tcpTransmitter READ tcpTransmitter CONSTANT)
    Q_PROPERTY(QVariant webSocketTransmitter READ webSocketTransmitter CONSTANT)
    Q_PROPERTY(QVariant serialPortTransmitter READ serialPortTransmitter CONSTANT)

    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
public:
    explicit SAKToolBox(QObject* parent = nullptr);
    ~SAKToolBox();

    /// You must call the interface before useing.
    Q_INVOKABLE void initialize(int type);

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void send(const QByteArray& bytes);
    bool isWorking();

    SAKCommunicationTool* getCommunicationTool();
    SAKEmitterTool* getEmitterTool();
    SAKResponserTool* getResponserTool();
    SAKStorerTool* getStorerTool();
    SAKPrestorerTool* getPrestorerTool();
    SAKVelometerTool* getRxVelometerTool();
    SAKVelometerTool* getTxVelometerTool();
    SAKStatisticianTool* getRxStatisticianTool();
    SAKStatisticianTool* getTxStatisticianTool();
    SAKUdpTransmitterTool* getUdpTransmitterTool();
    SAKTcpTransmitterTool* getTcpTransmitterTool();
    SAKWebSocketTransmitterTool* getWebSocketTransmitterTool();
    SAKSerialPortTransmitterTool* getSerialPortTransmitterTool();

signals:
    void errorOccurred(const QString& errorString);
    void isWorkingChanged();
    void communicatorChanged();

private:
    QList<SAKBaseTool*> m_tools;

private:
    void uninitializedTips();

private:
    SAKCommunicationTool* m_comunicator{nullptr};
    SAKEmitterTool* m_emitter{nullptr};
    SAKResponserTool* m_responser{nullptr};
    SAKStorerTool* m_storer{nullptr};
    SAKPrestorerTool* m_prestorer{nullptr};
    SAKVelometerTool* m_rxVelometer{nullptr};
    SAKVelometerTool* m_txVelometer{nullptr};
    SAKStatisticianTool* m_rxCounter{nullptr};
    SAKStatisticianTool* m_txCounter{nullptr};
    SAKUdpTransmitterTool* m_udpTransmitter{nullptr};
    SAKTcpTransmitterTool* m_tcpTransmitter{nullptr};
    SAKWebSocketTransmitterTool* m_webSocketTransmitter{nullptr};
    SAKSerialPortTransmitterTool* m_serialPortTransmitter{nullptr};
    bool m_isWorking{false};

private:
    QVariant communicator();
    QVariant emitter();
    QVariant responser();
    QVariant storer();
    QVariant prestorer();
    QVariant rxVelometer();
    QVariant txVelometer();
    QVariant rxStatistician();
    QVariant txStatistician();
    QVariant udpTransmitter();
    QVariant tcpTransmitter();
    QVariant webSocketTransmitter();
    QVariant serialPortTransmitter();

    void onCommunicatorBytesWritten(const QByteArray& bytes, const QString& to);
    void onCommunicatorBytesRead(const QByteArray& bytes, const QString& to);
};

#endif // SAKTOOLBOX_H
