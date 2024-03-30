/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>
#include <QVariant>

#include "xToolsBaseTool.h"
#include "xToolsCommunicationTool.h"
#include "xToolsEmitterTool.h"
#include "xToolsPrestorerTool.h"
#include "xToolsResponserTool.h"
#include "xToolsSerialPortTransmitterTool.h"
#include "xToolsStatisticianTool.h"
#include "xToolsStorerTool.h"
#include "xToolsTcpTransmitterTool.h"
#include "xToolsUdpTransmitterTool.h"
#include "xToolsVelometerTool.h"
#include "xToolsWebSocketTransmitterTool.h"

class xToolsToolBox : public QObject
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
    explicit xToolsToolBox(QObject* parent = nullptr);
    ~xToolsToolBox();

    /// You must call the interface before useing.
    Q_INVOKABLE void initialize(int type);

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    Q_INVOKABLE void send(const QByteArray& bytes);
    bool isWorking();

    xToolsCommunicationTool* getCommunicationTool();
    xToolsEmitterTool* getEmitterTool();
    xToolsResponserTool* getResponserTool();
    xToolsStorerTool* getStorerTool();
    xToolsPrestorerTool* getPrestorerTool();
    xToolsVelometerTool* getRxVelometerTool();
    xToolsVelometerTool* getTxVelometerTool();
    xToolsStatisticianTool* getRxStatisticianTool();
    xToolsStatisticianTool* getTxStatisticianTool();
    xToolsUdpTransmitterTool* getUdpTransmitterTool();
    xToolsTcpTransmitterTool* getTcpTransmitterTool();
    xToolsWebSocketTransmitterTool* getWebSocketTransmitterTool();
    xToolsSerialPortTransmitterTool* getSerialPortTransmitterTool();

signals:
    void errorOccurred(const QString& errorString);
    void isWorkingChanged();
    void communicatorChanged();

private:
    QList<xToolsBaseTool*> m_tools;

private:
    void uninitializedTips();

private:
    xToolsCommunicationTool* m_comunicator{nullptr};
    xToolsEmitterTool* m_emitter{nullptr};
    xToolsResponserTool* m_responser{nullptr};
    xToolsStorerTool* m_storer{nullptr};
    xToolsPrestorerTool* m_prestorer{nullptr};
    xToolsVelometerTool* m_rxVelometer{nullptr};
    xToolsVelometerTool* m_txVelometer{nullptr};
    xToolsStatisticianTool* m_rxCounter{nullptr};
    xToolsStatisticianTool* m_txCounter{nullptr};
    xToolsUdpTransmitterTool* m_udpTransmitter{nullptr};
    xToolsTcpTransmitterTool* m_tcpTransmitter{nullptr};
    xToolsWebSocketTransmitterTool* m_webSocketTransmitter{nullptr};
    xToolsSerialPortTransmitterTool* m_serialPortTransmitter{nullptr};
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
    void onCommunicatorBytesRead(const QByteArray& bytes, const QString& from);
};
