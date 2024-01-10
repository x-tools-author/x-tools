/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktoolbox.h"

#include "saktoolfactory.h"

SAKToolBox::SAKToolBox(QObject* parent)
    : QObject{parent}
{
    auto createTool = [](int type) -> SAKBaseTool* {
        auto toolFactory = SAKToolFactory::instance();
        return toolFactory->createTool(type);
    };

    // clang-format off
    m_emitterTool = qobject_cast<SAKEmitterTool*>(createTool(SAKToolFactory::EmitterTool));
    m_responserTool = qobject_cast<SAKResponserTool*>(createTool(SAKToolFactory::ResponserTool));
    m_storerTool = qobject_cast<SAKStorerTool*>(createTool(SAKToolFactory::StorerTool));
    m_prestorerTool = qobject_cast<SAKPrestorerTool*>(createTool(SAKToolFactory::PrestoreTool));
    m_rxVelometerTool = qobject_cast<SAKVelometerTool*>(createTool(SAKToolFactory::VelometerTool));
    m_txVelometerTool = qobject_cast<SAKVelometerTool*>(createTool(SAKToolFactory::VelometerTool));
    m_rxStatisticianTool = qobject_cast<SAKStatisticianTool*>(createTool(SAKToolFactory::StatistiticianTool));
    m_txStatisticianTool = qobject_cast<SAKStatisticianTool*>(createTool(SAKToolFactory::StatistiticianTool));

    m_udpTransmitterTool = qobject_cast<SAKUdpTransmitterTool*>(createTool(SAKToolFactory::UdpTransmitterTool));
    m_tcpTransmitterTool = qobject_cast<SAKTcpTransmitterTool*>(createTool(SAKToolFactory::TcpTransmitterTool));
    m_webSocketTransmitterTool = qobject_cast<SAKWebSocketTransmitterTool*>( createTool(SAKToolFactory::WebSocketTransmitterTool));
    m_serialPortTransmitterTool = qobject_cast<SAKSerialPortTransmitterTool*>( createTool(SAKToolFactory::SerialPortTransmitterTool));
    // clang-format on

    m_tools << m_emitterTool << m_responserTool << m_storerTool << m_prestorerTool
            << m_rxVelometerTool << m_txVelometerTool << m_rxStatisticianTool
            << m_txStatisticianTool << m_udpTransmitterTool << m_tcpTransmitterTool
            << m_webSocketTransmitterTool << m_serialPortTransmitterTool;

    int flag = Qt::AutoConnection | Qt::UniqueConnection;
    for (auto tool : m_tools) {
        connect(tool,
                &SAKBaseTool::errorOccurred,
                this,
                &SAKToolBox::errorOccurred,
                Qt::ConnectionType(flag));
    }

    connect(this, &SAKToolBox::errorOccurred, this, [=]() { this->close(); });
}

SAKToolBox::~SAKToolBox()
{
    if (m_comunicationTool) {
        m_comunicationTool->exit();
        m_comunicationTool->wait();
        m_comunicationTool->deleteLater();
    }

    for (auto tool : m_tools) {
        tool->exit();
        tool->wait();
        tool->deleteLater();
    }
}

void SAKToolBox::initialize(int type)
{
    if (m_comunicationTool) {
        m_comunicationTool->exit();
        m_comunicationTool->wait();
        m_comunicationTool->deleteLater();
        m_comunicationTool = nullptr;
    }

    auto toolFactory = SAKToolFactory::instance();
    auto tool = toolFactory->createTool(type);
    m_comunicationTool = qobject_cast<SAKCommunicationTool*>(tool);
    if (!m_comunicationTool) {
        qWarning() << "mComunicationTool is nullptr, type:" << type;
        return;
    }

    // clang-format off
    // rx->responser
    connect(m_comunicationTool, &SAKBaseTool::outputBytes, m_responserTool, &SAKBaseTool::inputBytes);
    // emiiter,responser,prestorer->tx
    connect(m_emitterTool, &SAKBaseTool::outputBytes, m_comunicationTool, &SAKBaseTool::inputBytes);
    connect(m_responserTool, &SAKBaseTool::outputBytes, m_comunicationTool, &SAKBaseTool::inputBytes);
    connect(m_prestorerTool, &SAKBaseTool::outputBytes, m_comunicationTool, &SAKBaseTool::inputBytes);
    // rx->storer; tx->storer
    connect(m_comunicationTool, &SAKBaseTool::outputBytes, m_storerTool, &SAKBaseTool::inputBytes);
    connect(m_comunicationTool, &SAKBaseTool::outputBytes, m_storerTool, &SAKBaseTool::inputBytes);
    // rx->velometer; tx->velometer
    connect(m_comunicationTool, &SAKBaseTool::outputBytes, m_rxVelometerTool, &SAKBaseTool::inputBytes);
    connect(m_comunicationTool, &SAKBaseTool::outputBytes, m_txVelometerTool, &SAKBaseTool::inputBytes);
    // rx->statistician; tx->statistician
    connect(m_comunicationTool, &SAKCommunicationTool::outputBytes, m_rxStatisticianTool, &SAKBaseTool::inputBytes);
    connect(m_comunicationTool, &SAKCommunicationTool::outputBytes, m_txStatisticianTool, &SAKBaseTool::inputBytes);
    // rx->serialport transmition; serialport transmition->Tx analyzer
    connect(m_comunicationTool, &SAKBaseTool::outputBytes, m_serialPortTransmitterTool, &SAKBaseTool::inputBytes);
    connect(m_serialPortTransmitterTool, &SAKBaseTool::outputBytes, m_comunicationTool, &SAKBaseTool::inputBytes);
    // rx->udp transmition; udp transmition->Tx analyzer
    connect(m_comunicationTool, &SAKBaseTool::outputBytes, m_udpTransmitterTool, &SAKBaseTool::inputBytes);
    connect(m_udpTransmitterTool, &SAKBaseTool::outputBytes, m_comunicationTool, &SAKBaseTool::inputBytes);
    // rx->tcp transmition; tcp transmition->Tx analyzer
    connect(m_comunicationTool, &SAKBaseTool::outputBytes, m_tcpTransmitterTool, &SAKBaseTool::inputBytes);
    connect(m_tcpTransmitterTool, &SAKBaseTool::outputBytes, m_comunicationTool, &SAKBaseTool::inputBytes);
    // rx->websocket transmition; websocket transmition->Tx analyzer
    connect(m_comunicationTool, &SAKBaseTool::outputBytes, m_webSocketTransmitterTool, &SAKBaseTool::inputBytes);
    connect(m_webSocketTransmitterTool, &SAKBaseTool::outputBytes, m_comunicationTool, &SAKBaseTool::inputBytes);

    connect(m_comunicationTool, &SAKCommunicationTool::errorOccurred, this, &SAKToolBox::errorOccurred);
    // clang-format on

    emit communicatonChanged();
}

void SAKToolBox::open()
{
    if (m_comunicationTool) {
        for (auto tool : m_tools) {
            tool->start();
        }

        m_isWorking = true;
        emit isWorkingChanged();
        m_comunicationTool->start();
    } else {
        uninitializedTips();
    }
}

void SAKToolBox::close()
{
    if (m_comunicationTool) {
        m_comunicationTool->exit();
        m_comunicationTool->wait();

        for (auto tool : m_tools) {
            tool->exit();
            tool->wait();
        }

        m_isWorking = false;
        emit isWorkingChanged();
    } else {
        uninitializedTips();
    }
}

void SAKToolBox::send(const QByteArray& bytes)
{
    m_comunicationTool->inputBytes(bytes);
}

bool SAKToolBox::isWorking()
{
    return m_isWorking;
}

SAKCommunicationTool* SAKToolBox::getCommunicationTool()
{
    return m_comunicationTool;
}

SAKEmitterTool* SAKToolBox::getEmitterTool()
{
    return m_emitterTool;
}

SAKResponserTool* SAKToolBox::getResponserTool()
{
    return m_responserTool;
}

SAKStorerTool* SAKToolBox::getStorerTool()
{
    return m_storerTool;
}

SAKPrestorerTool* SAKToolBox::getPrestorerTool()
{
    return m_prestorerTool;
}

SAKVelometerTool* SAKToolBox::getRxVelometerTool()
{
    return m_rxVelometerTool;
}

SAKVelometerTool* SAKToolBox::getTxVelometerTool()
{
    return m_txVelometerTool;
}

SAKStatisticianTool* SAKToolBox::getRxStatisticianTool()
{
    return m_rxStatisticianTool;
}

SAKStatisticianTool* SAKToolBox::getTxStatisticianTool()
{
    return m_txStatisticianTool;
}

SAKUdpTransmitterTool* SAKToolBox::getUdpTransmitterTool()
{
    return m_udpTransmitterTool;
}

SAKTcpTransmitterTool* SAKToolBox::getTcpTransmitterTool()
{
    return m_tcpTransmitterTool;
}

SAKWebSocketTransmitterTool* SAKToolBox::getWebSocketTransmitterTool()
{
    return m_webSocketTransmitterTool;
}

SAKSerialPortTransmitterTool* SAKToolBox::getSerialPortTransmitterTool()
{
    return m_serialPortTransmitterTool;
}

void SAKToolBox::uninitializedTips()
{
    static QByteArray tips("You must call the interface name initialize()"
                           " before using the object.");
    Q_ASSERT_X(false, __FUNCTION__, tips.constData());

    emit errorOccurred(QString::fromLatin1(tips));
}

QVariant SAKToolBox::communicaton()
{
    return QVariant::fromValue(m_comunicationTool);
}

QVariant SAKToolBox::emitter()
{
    return QVariant::fromValue(m_emitterTool);
}

QVariant SAKToolBox::responser()
{
    return QVariant::fromValue(m_responserTool);
}

QVariant SAKToolBox::storer()
{
    return QVariant::fromValue(m_storerTool);
}

QVariant SAKToolBox::prestorer()
{
    return QVariant::fromValue(m_prestorerTool);
}

QVariant SAKToolBox::rxVelometer()
{
    return QVariant::fromValue(m_rxVelometerTool);
}

QVariant SAKToolBox::txVelometer()
{
    return QVariant::fromValue(m_txVelometerTool);
}

QVariant SAKToolBox::rxStatistician()
{
    return QVariant::fromValue(m_rxStatisticianTool);
}

QVariant SAKToolBox::txStatistician()
{
    return QVariant::fromValue(m_txStatisticianTool);
}

QVariant SAKToolBox::udpTransmitter()
{
    return QVariant::fromValue(m_udpTransmitterTool);
}

QVariant SAKToolBox::tcpTransmitter()
{
    return QVariant::fromValue(m_tcpTransmitterTool);
}

QVariant SAKToolBox::webSocketTransmitter()
{
    return QVariant::fromValue(m_webSocketTransmitterTool);
}

QVariant SAKToolBox::serialPortTransmitter()
{
    return QVariant::fromValue(m_serialPortTransmitterTool);
}
